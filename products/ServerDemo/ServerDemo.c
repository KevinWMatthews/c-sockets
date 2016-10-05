#include <stdio.h>
#include "Socket.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>


typedef struct ProgramOptionsStruct * ProgramOptions;
typedef struct ProgramOptionsStruct
{
    char ip_address[16];
    int port;
} ProgramOptionsStruct;

static void parse_options(ProgramOptions options, int argc, char * argv[])
{
    int i = 0;
    char port[6] = {0};

    if (options == 0)
    {
        printf("%s was passed a null pointer!\n", __func__);
        return;
    }

    for (i = 0; i < argc; i++)
    {
        if ( strcmp(argv[i], "--ip-address") == 0 )
        {
            i++;
            strcpy(options->ip_address, argv[i]);
        }
        if ( strcmp(argv[i], "--port") == 0 )
        {
            i++;
            strcpy(port, argv[i]);
            options->port = atoi(port);
        }
    }
}

static void close_and_destroy_socket(Socket *socket)
{
    printf("Closing socket...\n");
    Socket_Close(*socket);
    Socket_Destroy(socket);
}

static void * socket_handler_thread(void * client_socket)
{
    Socket socket = *(Socket *)client_socket;
    char * confirm_connection = "The server has accepted the connection.\n";
    char buffer[2000] = {0};
    int return_code = SOCKET_FAIL;

    if ( Socket_Send( socket, confirm_connection, strlen(confirm_connection) ) < 0 )
    {
        perror("Send failed");
        close_and_destroy_socket(&socket);
        exit(EXIT_FAILURE);
        return NULL;
    }

    while (1)
    {
        memset( buffer, 0, sizeof(buffer) );
        return_code = Socket_Receive( socket, buffer, sizeof(buffer) );
        if (return_code < 0)
        {
            perror("Receive failure");
            pthread_exit( (void *)&return_code );
        }
        else if (return_code == 0)
        {
            printf("Socket closed!\n");
            pthread_exit( (void *)&return_code );
        }
        printf( "Received message from %s:%d, %d:\n", Socket_GetIpAddress(socket), Socket_GetPort(socket), Socket_GetDescriptor(socket) );
        printf("%s\n", buffer);
        printf("Sending response to client... ");
        if ( Socket_Send(socket, buffer, sizeof(buffer) ) < 0 )
        {
            perror("Send failed");
            close_and_destroy_socket(&socket);
            exit(EXIT_FAILURE);
            return NULL;
        }
        printf("message sent.\n");
    }

    return_code = 0;
    pthread_exit( (void *)&return_code );
    return NULL;
}

int main(int argc, char * argv[])
{
    Socket server_socket = 0;
    SocketSettingsStruct server_socket_settings = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_STREAM,
        .protocol = SOCKET_PROTOCOL_DEFAULT
    };
    SocketOptionsStruct server_socket_options = {
        .option_name = SOCKET_OPTION_REUSE_ADDRESS
    };
    Socket client_socket = 0;
    ProgramOptionsStruct user_options = {
        .ip_address = "127.0.0.1",
        .port = 8888
    };
    int backlog = 0;

    parse_options(&user_options, argc, argv);

    printf("Starting server at %s:%d...\n", user_options.ip_address, user_options.port);

    printf("Creating server socket...\n");
    server_socket = Socket_Create();
    if (server_socket == 0)
    {
        printf("Could not create server Socket!\n");
        return 1;
    }

    printf("Opening socket...\n");
    if ( Socket_Open(server_socket, &server_socket_settings) < 0 )
    {
        perror("Open failed");
        Socket_Destroy(&server_socket);
        return 1;
    }

    printf("Setting socket options...\n");
    if ( Socket_SetOptions(server_socket, &server_socket_options) < 0 )
    {
        perror("SetOptions failed");
        close_and_destroy_socket(&server_socket);
        return 1;
    }

    printf("Binding socket...\n");
    if ( Socket_Bind(server_socket, user_options.ip_address, user_options.port) < 0 )
    {
        perror("Bind failed");
        close_and_destroy_socket(&server_socket);
        return 1;
    }

    printf("Listening on socket...\n");
    if ( Socket_Listen(server_socket, backlog) < 0 )
    {
        perror("Listen failed");
        close_and_destroy_socket(&server_socket);
        return 1;
    }

    printf("Waiting to accept a connection...\n");
    printf("To connect to this socket, open a new terminal window and type:\ntelnet %s %d\n", user_options.ip_address, user_options.port);
    do
    {
        client_socket = Socket_Accept(server_socket);
        if (!client_socket)
        {
            perror("Accept failed");
            close_and_destroy_socket(&server_socket);
            return 1;
        }
        else
        {
            pthread_t thread_handle;
            if ( pthread_create( &thread_handle, NULL, socket_handler_thread, (void *)&client_socket) < 0)
            {
                printf("Failed to create thread\n");
                close_and_destroy_socket(&server_socket);
                close_and_destroy_socket(&client_socket);
                return 1;
            }
            pthread_detach(thread_handle);
        }
        printf("Connection accepted.\n");
    } while (1);

    printf("Closing and destroying sockets...\n");
    close_and_destroy_socket(&server_socket);
    close_and_destroy_socket(&client_socket);

    printf("Server stopping!\n");
    return 0;
}
