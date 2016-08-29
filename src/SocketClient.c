#include "SocketClient.h"
#include "UnixSocket.h"

int Socket_Connect(Socket self, const char * ip_address, int port)
{
    int result = SOCKET_FAIL;
    if (self == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface->connect == 0)
        return SOCKET_NULL_POINTER;

    if (ip_address == 0)
        return SOCKET_NULL_POINTER;

    result = self->interface->connect(self->file_descriptor, ip_address, port);
    if ( result < 0 )
    {
        return result;
    }
    self->ip_address = ip_address;
    self->port = port;
    return SOCKET_SUCCESS;
}
