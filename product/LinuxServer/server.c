#include <stdio.h>
#include "Socket.h"

int main(int argc, char * argv[])
{
    Socket socket = Socket_Create();
    char * message = "Hello, world!\n";
    if ( Socket_Open(socket) < 0 )
    {
        printf("Could not open socketn");
    }
    Socket_Connect(socket, "10.0.1.34", 10004);
    if ( Socket_Send(socket, message, sizeof(message) ) )
    {
        printf("Could not send message to socket\n");
    }
    Socket_Close(socket);
    Socket_Destroy(&socket);
    return 0;
}
