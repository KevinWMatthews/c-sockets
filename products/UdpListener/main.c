#include <stdio.h>
#include <Socket.h>
#include <string.h>
#include <stdlib.h>

static void parse_options(SocketAddress socket_address, int argc, char * argv[])
{
    int i = 0;
    char port[6] = {0};

    if (socket_address == 0)
    {
        printf("%s was passed a null pointer!\n", __func__);
        return;
    }

    for (i = 0; i < argc; i++)
    {
        if ( strcmp(argv[i], "--ip-address") == 0 )
        {
            i++;
            strcpy(socket_address->ip_address, argv[i]);
        }
        if ( strcmp(argv[i], "--port") == 0 )
        {
            i++;
            strcpy(port, argv[i]);
            socket_address->port = atoi(port);
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
    SocketAddressStruct socket_address = {
        .ip_address = "127.0.0.1",
        .port = 8888
    };

    char buffer[2000] = {0};

    parse_options(&socket_address, argc, argv);

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
