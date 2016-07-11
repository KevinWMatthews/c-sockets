#include <stdio.h>
#include "Socket.h"

int main(int argc, char * argv[])
{
    Socket socket = Socket_Create();
    char message[20+1] = {0};
    if ( Socket_Open(socket) < 0 )
    {
        printf("Could not open socketn");
    }
    if ( Socket_Receive(socket, message, sizeof(message) ) )
    {
        printf("Could not receive message to socket\n");
    }
    Socket_Close(socket);
    Socket_Destroy(&socket);
    return 0;
}
