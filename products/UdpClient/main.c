#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Socket.h"
#include <argp.h>

static char doc[] = "UDP client socket demo.\nSend a single UDP packet to a remote socket.";
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

int main(int argc, char * argv[])
{
    struct arguments arguments = {
        .ip_address = "127.0.0.1"
    };
    Socket client = {0};
    SocketSettingsStruct client_settings = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_DATAGRAM,
        .protocol = SOCKET_PROTOCOL_DEFAULT
    };
    int return_code = 0;
    char test_message[] = {"This is a test.\n"};
    SocketAddressStruct socket_address;

    // Parse arguments
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    socket_address.ip_address = arguments.ip_address;
    socket_address.port = atoi(arguments.args[ARGUMENT_INDEX_PORT]);

    printf("Starting UDP Client...\n");
    client = Socket_Create();
    if (client == 0)
    {
        printf("Could not create socket!\n");
        return 1;
    }

    printf("Opening socket...\n");
    return_code = Socket_Open(client, &client_settings);
    if (return_code < 0)
    {
        if (return_code == SOCKET_FAILED_SYSTEM_CALL)
            perror("System call to socket open failed");
        else
            printf("Could not open socket!\n");

        printf("Destroying socket...\n");
        Socket_Close(client);
        Socket_Destroy(&client);
    }

    printf("Connecting to socket...\n");
    return_code = SocketClient_Connect(client, &socket_address);
    if (return_code < 0)
    {
        if (return_code == SOCKET_FAILED_SYSTEM_CALL)
            perror("System call to socket connect failed");
        else
            printf("Could not connect socket!\n");

        printf("Destroying socket...\n");
        Socket_Close(client);
        Socket_Destroy(&client);
    }

    printf("Sending data to socket at %s:%d...\n", socket_address.ip_address, socket_address.port);
    return_code = Socket_Send( client, test_message, sizeof(test_message) );
    if (return_code < 0)
    {
        if (return_code == SOCKET_FAILED_SYSTEM_CALL)
            perror("System call to socket send failed");
        else
            printf("Could not send to socket!\n");

        printf("Destroying socket...\n");
        Socket_Close(client);
        Socket_Destroy(&client);
    }

    printf("Closing socket...\n");
    Socket_Close(client);
    printf("Destroying socket...\n");
    Socket_Destroy(&client);

    printf("Stopping UDP Client!\n");
    return 0;
}
