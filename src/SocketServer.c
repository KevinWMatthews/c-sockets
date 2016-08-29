#include "SocketServer.h"
#include "UnixSocket.h"

int Socket_Bind(Socket self, const char * ip_address, int port)
{
    int result = SOCKET_FAIL;

    if (self == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface->bind == 0)
        return SOCKET_NULL_POINTER;

    result = self->interface->bind(self->file_descriptor, ip_address, port);
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
        return SOCKET_NULL_POINTER;
    if (self->interface == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface->listen == 0)
        return SOCKET_NULL_POINTER;
    return self->interface->listen(self->file_descriptor, backlog);
}

Socket Socket_Accept(Socket self)
{
    Socket new_socket = {0};
    int new_file_descriptor = UNIX_SOCKET_FAIL;

    if (self == 0)
        return 0;
    if (self->interface == 0)
        return 0;
    if (self->interface->accept == 0)
        return 0;

    new_file_descriptor = self->interface->accept(self->file_descriptor);
    if (new_file_descriptor < 0)
    {
        return 0;
    }
    new_socket = Socket_Create(self->interface);    // Both sockets must have the same interface?
    new_socket->file_descriptor = new_file_descriptor;
    return new_socket;
}
