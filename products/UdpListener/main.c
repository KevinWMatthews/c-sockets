#include <stdio.h>
#include <Socket.h>
#include <string.h>
#include <stdlib.h>

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

int main(int argc, char * argv[])
{
    Socket socket;
    SocketSettingsStruct socket_settings = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_DATAGRAM,
        .protocol = SOCKET_PROTOCOL_UDP
    };
    SocketOptionsStruct socket_options = {
        .option_name = SOCKET_OPTION_UDP_BROADCAST
    };
    ProgramOptionsStruct user_options = {
        .ip_address = "127.0.0.1",
        .port = 8888
    };

    char buffer[2000] = {0};

    parse_options(&user_options, argc, argv);
    // printf("Enter an IP address to listen to:\n");
    // scanf("%s", ip_address);
    // printf("Enter a port to listen to:\n");
    // scanf("%d", &port);

    printf("Starting UDP listener on %s:%d...\n", user_options.ip_address, user_options.port);

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

    printf("Setting socket options...\n");
    if ( Socket_SetOptions(socket, &socket_options) < 0 )
    {
        perror("Could not set socket options");
        Socket_Close(socket);
        Socket_Destroy(&socket);
        return 1;
    }

    printf("Binding socket...\n");
    if ( Socket_Bind(socket, user_options.ip_address, user_options.port) < 0 )
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
