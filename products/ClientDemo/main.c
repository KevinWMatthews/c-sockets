#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Socket.h"
#include <argp.h>

static char doc[] = "TCP client socket demo.\nRead input from the terimal and send it to a remote socket.";
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

int main(int argc, char * argv[])
{
    struct arguments arguments = {
        .ip_address = "127.0.0.1"
    };
    Socket client = 0;
    SocketSettingsStruct client_settings = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_STREAM,
        .protocol = SOCKET_PROTOCOL_TCP
    };
    char receive_buffer[2000] = {0};
    char message_buffer[1024] = {0};
    int return_code = 0;
    SocketAddressStruct socket_address;

    // Parse arguments
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    socket_address.ip_address = arguments.ip_address;
    socket_address.port = atoi(arguments.args[ARGUMENT_INDEX_PORT]);

    printf("Connecting client to %s:%d...\n", socket_address.ip_address, socket_address.port);

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

    return_code = SocketClient_Connect(client, &socket_address);
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
