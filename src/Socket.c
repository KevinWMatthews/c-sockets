#include "Socket.h"
#include "UnixSocket.h"
#include <stdlib.h>

static SocketInterfaceStruct interface;

Socket Socket_Create(void)
{
    Socket self = calloc( 1, sizeof(*self) );
    if (self == 0)
    {
        return 0;
    }
    self->port = SOCKET_INVALID_PORT;
    // set the interface here depending on the interface type
    interface.open = UnixSocket_Open;
    interface.close = UnixSocket_Close;
    interface.send = UnixSocket_Send;
    interface.receive = UnixSocket_Receive;
    self->interface = &interface;
    return self;
}

void Socket_Destroy(Socket * self)
{
    if (self == 0)
    {
        return;
    }
    free(*self);
    *self = 0;
}

int Socket_Open(Socket self)
{
    int file_descriptor = 0;
    if (self == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface->open == 0)
        return SOCKET_NULL_POINTER;

    file_descriptor = self->interface->open();
    self->file_descriptor = file_descriptor;
    if (self->file_descriptor < 0)
    {
        return SOCKET_INVALID_FILE_DESCRIPTOR;
    }
    return SOCKET_SUCCESS;
}

void Socket_Close(Socket self)
{
    if (self == 0)
        return;
    if (self->interface == 0)
        return;
    if (self->interface->close == 0)
        return;
    self->interface->close(self->file_descriptor);
    self->file_descriptor = SOCKET_INVALID_FILE_DESCRIPTOR;
    self->ip_address = 0;
    self->port = SOCKET_INVALID_PORT;
}

int Socket_GetFileDescriptor(Socket self)
{
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    return self->file_descriptor;
}

const char * Socket_GetIpAddress(Socket self)
{
    if (self == 0)
    {
        return 0;
    }
    return self->ip_address;
}

int Socket_GetPort(Socket self)
{
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    return self->port;
}

int Socket_Send(Socket self, const char * message, unsigned int message_length)
{
    if (self == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface->send == 0)
        return SOCKET_NULL_POINTER;

    if (message == 0)
        return SOCKET_NULL_POINTER;
    if (message_length == 0)
        return SOCKET_FAIL;

    return self->interface->send(self->file_descriptor, message, message_length);
}

int Socket_Receive(Socket self, char * buffer, unsigned int buffer_length)
{
    if (self == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface == 0)
        return SOCKET_NULL_POINTER;
    if (self->interface->receive == 0)
        return SOCKET_NULL_POINTER;

    if (buffer == 0)
        return SOCKET_NULL_POINTER;
    if (buffer_length == 0)
        return SOCKET_FAIL;

    return self->interface->receive(self->file_descriptor, buffer, buffer_length);
}

int Socket_SetOption(Socket self, SocketOption option)
{
    UnixSocketOption unix_option = 0;
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }

    switch (option)
    {
        case SOCKET_IMMEDIATELY_REUSE_SOCKET:
            unix_option = UNIX_SOCKET_IMMEDIATELY_REUSE_SOCKET;
    }

    return UnixSocket_SetOption(self->file_descriptor, unix_option);
}
