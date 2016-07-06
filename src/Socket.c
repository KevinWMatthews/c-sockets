#include "Socket.h"
#include "UnixSocket.h"

int Socket_Open(void)
{
    return UnixSocket_Open();
}

void Socket_Close(int file_descriptor)
{
    UnixSocket_Close(file_descriptor);
}
