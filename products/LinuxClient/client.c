#include <stdio.h>
#include "Socket.h"
#include <string.h>

int main(void)
{
    Socket socket = Socket_Create();
    char * ip_address = "216.58.194.164";   // google
    int port = 80;
    char * message = "GET / HTTP/1.1\r\n\r\n";
    char receive_buffer[2048] = {0};

    printf("Opening client socket...\n");
    if ( Socket_Open(socket) < 0 )
    {
        printf("Could not open socket!\n");
        return 0;
    }

    if ( Socket_Connect(socket, ip_address, port) < 0 )
    {
        printf("Cound not connect to socket!\n");
    }
    printf("Socket connected.\n");

    if ( Socket_Send(socket, message, strlen(message)) < 0 )
    {
        printf("Cound not send message to socket!\n");
    }
    printf("Message sent.\n");

    if ( Socket_Receive(socket, receive_buffer, sizeof(receive_buffer) ) < 0 )
    {
        printf("Could not receive message to socket!\n");
    }
    printf("Message received: \n%s\n", receive_buffer);

    printf("Closing client socket...\n");
    Socket_Close(socket);
    Socket_Destroy(&socket);

    return 0;
}
