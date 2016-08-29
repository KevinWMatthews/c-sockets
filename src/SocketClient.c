#include "SocketClient.h"
#include "UnixSocket.h"

int Socket_Connect(Socket self, const char * ip_address, int port)
{
    int result = SOCKET_FAIL;
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    if (ip_address == 0)
    {
        return SOCKET_NULL_POINTER;
    }

    result = UnixSocket_Connect(self->file_descriptor, ip_address, port);
    if ( result < 0 )
    {
        return result;
    }
    self->ip_address = ip_address;
    self->port = port;
    return SOCKET_SUCCESS;
}
