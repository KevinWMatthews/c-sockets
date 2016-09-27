#include <stdio.h>
#include <Socket.h>
#include <string.h>

int main(void)
{
    Socket socket;
    SocketSettingsStruct socket_settings = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_DATAGRAM,
        .protocol = SOCKET_PROTOCOL_UDP
    };
    char buffer[2000] = {0};

    printf("Starting UDP listener...\n");

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

    while (1)
    {
        memset( buffer, 0, sizeof(buffer) );
        Socket_Receive( socket, buffer, sizeof(buffer) );
        printf("Received data:\n");
        printf("%s\n", buffer);
    }

    printf("Closing socket...\n");
    Socket_Close(socket);

    printf("Tearing down Socket...\n");
    Socket_Destroy(&socket);
    printf("Stopping UDP listener!\n");
    return 0;
}
