#include "Socket.h"
#include "UnixSocket.h"

static int file_descriptor;

int Socket_Open(void)
{
    file_descriptor = UnixSocket_Open();
    if ( file_descriptor < 0)
    {
        return file_descriptor;
    }
    return SOCKET_SUCCESS;
}

void Socket_Close(void)
{
    UnixSocket_Close(file_descriptor);
}
