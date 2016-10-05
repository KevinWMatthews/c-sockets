#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Socket.h"


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

int main(int argc, char * argv[])
{
    Socket client = 0;
    SocketSettingsStruct client_settings = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_STREAM,
        .protocol = SOCKET_PROTOCOL_TCP
    };
    char receive_buffer[2000] = {0};
    char message_buffer[1024] = {0};
    int return_code = 0;
    ProgramOptionsStruct user_options = {
        .ip_address = "127.0.0.1",
        .port = 8888
    };

    parse_options(&user_options, argc, argv);

    printf("Connecting client to %s:%d...\n", user_options.ip_address, user_options.port);

    client = Socket_Create();
    if (client == 0)
    {
        printf("Could not create client socket!\n");
        return 1;
    }

    return_code = Socket_Open(client, &client_settings);
    if (return_code < 0)
    {
        if (return_code == SOCKET_FAILED_SYSTEM_CALL)
            perror("System call to socket open failed");
        else
            printf("Socket_Open failed: %d.\n", return_code);

        printf("Destroying socket...\n");
        Socket_Destroy(&client);
        return 1;
    }

    return_code = SocketClient_Connect(client, user_options.ip_address, user_options.port);
    if (return_code < 0)
    {
        if (return_code == SOCKET_FAILED_SYSTEM_CALL)
            perror("System call to socket connect failed");
        else
            printf("SocketClient_Connect failed: %d.\n", return_code);

        close_and_destroy_socket(&client);
        return 1;
    }

    return_code = Socket_Receive( client, receive_buffer, sizeof(receive_buffer) );
    if (return_code < 0)
    {
        if (return_code == SOCKET_FAILED_SYSTEM_CALL)
            perror("System call to socket receive failed");
        else
            printf("Socket_Receive failed: %d.\n", return_code);

        close_and_destroy_socket(&client);
        return 1;
    }
    printf("Received message:\n%s\n", receive_buffer);

    while (1)
    {
        printf("Enter a message to send to the server:\n");
        scanf("%s", message_buffer);

        return_code = Socket_Send( client, message_buffer, sizeof(message_buffer) );
        if ( return_code < 0 )
        {
            if (return_code == SOCKET_FAILED_SYSTEM_CALL)
                perror("System call to socket send failed");
            else
                printf("Socket_Send failed: %d.\n", return_code);

            close_and_destroy_socket(&client);
            return 1;
        }

        memset( receive_buffer, 0, sizeof(receive_buffer) );
        return_code = Socket_Receive( client, receive_buffer, sizeof(receive_buffer) );
        if (return_code == 0)
        {
            printf("Socket terminated by server!\n");
            printf("Closing and destroying client sockets\n");
            close_and_destroy_socket(&client);
            return 1;
        }
        else if (return_code < 0)
        {
            if (return_code == SOCKET_FAILED_SYSTEM_CALL)
                perror("System call to socket receive failed");
            else
                printf("Socket_Receive failed: %d.\n", return_code);

            close_and_destroy_socket(&client);
            return 1;
        }
        printf("Received message:\n%s\n", receive_buffer);
    }

    printf("Closing and destroying client sockets\n");
    close_and_destroy_socket(&client);

    printf("Client stopping!\n");
    return 0;
}
