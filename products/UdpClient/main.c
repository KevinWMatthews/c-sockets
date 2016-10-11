#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Socket.h"

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
    Socket client = {0};
    SocketSettingsStruct client_settings = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_DATAGRAM,
        .protocol = SOCKET_PROTOCOL_DEFAULT
    };
    int return_code = 0;
    char test_message[] = {"This is a test.\n"};
    SocketAddressStruct socket_address = {
        .ip_address = "127.0.0.1",
        .port = 8888
    };

    parse_options(&socket_address, argc, argv);

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
