#include <stdio.h>
#include <Socket.h>
#include <string.h>
#include <stdlib.h>
#include <argp.h>

static char doc[] = "Simple UDP listener.\nWaits for UDP packets and displays them on the console.";
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
    Socket socket;
    SocketSettingsStruct socket_settings = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_DATAGRAM,
        .protocol = SOCKET_PROTOCOL_UDP
    };
    SocketAddressStruct socket_address;
    char buffer[2000] = {0};

    // Parse arguments
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    socket_address.ip_address = arguments.ip_address;
    socket_address.port = atoi(arguments.args[ARGUMENT_INDEX_PORT]);

    printf("Starting UDP listener on %s:%d...\n", socket_address.ip_address, socket_address.port);

    printf("Creating Socket...\n");
    socket = Socket_Create();
    if (socket == 0)
    {
        printf("Could not create socket!\n");
        return 1;
    }

    printf("Opening socket...\n");
    if ( Socket_Open(socket, &socket_settings) < 0 )
    {
        perror("Could not open socket");
        Socket_Destroy(&socket);
        return 1;
    }

    printf("Binding socket...\n");
    if ( SocketServer_Bind(socket, &socket_address) < 0 )
    {
        perror("Could not bind socket");
        Socket_Close(socket);
        Socket_Destroy(&socket);
        return 1;
    }

    /* nc:
     *      nc      netcat
     *      -u      UDP packets
     *      -w1     wait 1 second for more data (it will wait forever otherwise).
     *
     * echo:
     *      -n      Do not send newline.
     */
    printf("To send data to this socket, open a new terminal window and type:\necho -n '<your_data_here>' | nc -u -w1 <ip_address> <port>\n\n");
    printf("Waiting to receive data...\n");
    while (1)
    {
        memset( buffer, 0, sizeof(buffer) );
        // The socket is bound so we can use the default receive.
        Socket_Receive( socket, buffer, sizeof(buffer) );
        printf("%s\n", buffer);
    }

    printf("Closing socket...\n");
    Socket_Close(socket);

    printf("Tearing down Socket...\n");
    Socket_Destroy(&socket);
    printf("Stopping UDP listener!\n");
    return 0;
}
