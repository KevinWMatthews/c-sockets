#include <stdio.h>
#include "Socket.h"

int main(int argc, char * argv[])
{
    Socket socket = Socket_Create();

    printf("Opening socket...\n");
    if ( Socket_Open(socket) < 0 )
    {
        printf("Open failed!\n");
    }

    printf("Binding socket...\n");
    if ( Socket_Bind(socket, 0, 8888) < 0 )
    {
        printf("Bind failed!\n");
    }

    printf("Listening on socket...\n");
    if ( Socket_Listen(socket, 0) < 0 ) //TODO experiment with the backlog?
    {
        printf("Listen failed!\n");
    }

    printf("Closing socket...\n");
    Socket_Close(socket);
    Socket_Destroy(&socket);

    return 0;
}
