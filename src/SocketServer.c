#include "SocketServer.h"
#include "UnixSocket.h"

int Socket_Bind(Socket self, const char * ip_address, int port)
{
    int result = SOCKET_FAIL;

    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }

    result = UnixSocket_Bind(self->file_descriptor, ip_address, port);
    if (result < 0)
    {
        return SOCKET_FAIL;
    }
    self->ip_address = ip_address;
    self->port = port;
    return result;
}

int Socket_Listen(Socket self, int backlog)
{
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    return UnixSocket_Listen(self->file_descriptor, backlog);
}

Socket Socket_Accept(Socket self)
{
    Socket new_socket = {0};
    int file_descriptor = UNIX_SOCKET_FAIL;

    if (self == 0)
    {
        return 0;
    }

    file_descriptor = UnixSocket_Accept(self->file_descriptor);
    if (file_descriptor < 0)
    {
        return 0;
    }
    new_socket = Socket_Create();
    new_socket->file_descriptor = file_descriptor;
    return new_socket;
}
