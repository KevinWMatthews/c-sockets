#include <stdio.h>
#include "Socket.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <argp.h>

static char doc[] = "Simple TCP server.\nAccepts multiple connections and displays received data on the console.";
static char args_doc[] = "PORT";

// keys may not be 0
#define ARGUMENT_KEY_IP_ADDRESS 1
static struct argp_option argp_options[] = {
    // long option, key, doc?, option options (haha), group
    { "ip-address", ARGUMENT_KEY_IP_ADDRESS, "IP_ADDRESS", 0, "IP address of remote socket", 0 },
    { 0 }
};

#define NUMBER_OF_ARGUMENTS 1
#define ARGUMENT_INDEX_PORT 0
struct arguments
{
    char * args[NUMBER_OF_ARGUMENTS];
    char * ip_address;
};

static error_t parse_arguments(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
        case ARGUMENT_KEY_IP_ADDRESS:
            if (arg)
                arguments->ip_address = arg;
            break;

        case ARGP_KEY_ARG:
        {
            unsigned index_of_argument = state->arg_num;
            if (index_of_argument >= NUMBER_OF_ARGUMENTS)
            {
                // Too many arguments; throw a usage error.
                argp_usage(state);
            }
            // Store the current argument.
            arguments->args[index_of_argument] = arg;
            break;
        }

        case ARGP_KEY_END:
        {
            unsigned index_of_argument = state->arg_num;
            if (index_of_argument < NUMBER_OF_ARGUMENTS)
            {
                // Not enough arguments; throw a usage error.
                argp_usage(state);
            }
            break;
        }

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {argp_options, parse_arguments, args_doc, doc, 0, 0, 0};

static void close_and_destroy_socket(Socket *socket)
{
    printf("Closing socket...\n");
    Socket_Close(*socket);
    Socket_Destroy(socket);
}

static void * socket_handler_thread(void * client_socket)
{
    Socket socket = *(Socket *)client_socket;
    char buffer[2000] = {0};
    int return_code = SOCKET_FAILED_SYSTEM_CALL;

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
        printf("%s\n", buffer);
    }

    return_code = 0;
    pthread_exit( (void *)&return_code );
    return NULL;
}

int main(int argc, char * argv[])
{
    struct arguments arguments = {
        .ip_address = "127.0.0.1"
    };
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
    SocketAddressStruct socket_address = {
        .ip_address = "127.0.0.1",
        .port = 8888
    };
    int backlog = 0;

    // Parse arguments
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    socket_address.ip_address = arguments.ip_address;
    socket_address.port = atoi(arguments.args[ARGUMENT_INDEX_PORT]);

    printf("Starting server at %s:%d...\n", socket_address.ip_address, socket_address.port);

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
        perror("Set options failed");
        close_and_destroy_socket(&server_socket);
        return 1;
    }

    printf("Binding socket...\n");
    if ( SocketServer_Bind(server_socket, &socket_address) < 0 )
    {
        perror("Bind failed");
        close_and_destroy_socket(&server_socket);
        return 1;
    }

    printf("Listening on socket...\n");
    if ( SocketServer_Listen(server_socket, backlog) < 0 )
    {
        perror("Listen failed");
        close_and_destroy_socket(&server_socket);
        return 1;
    }

    printf("Waiting to accept a connection...\n");
    printf("To connect to this socket, open a new terminal window and type:\ntelnet %s %d\n", socket_address.ip_address, socket_address.port);
    do
    {
        client_socket = SocketServer_Accept(server_socket);
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
