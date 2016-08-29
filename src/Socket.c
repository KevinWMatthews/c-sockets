#include "Socket.h"
#include "UnixSocket.h"
#include <stdlib.h>

Socket Socket_Create(void)
{
    Socket self = calloc( 1, sizeof(*self) );
    if (self == 0)
    {
        return 0;
    }
    self->port = SOCKET_INVALID_PORT;
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
    {
        return SOCKET_NULL_POINTER;
    }
    file_descriptor = UnixSocket_Open();
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
    {
        return;
    }
    UnixSocket_Close(self->file_descriptor);
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
    int file_descriptor = SOCKET_FAIL;
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    if (message == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    if (message_length == 0)
    {
        return SOCKET_FAIL;
    }

    file_descriptor = self->file_descriptor;
    return UnixSocket_Send(file_descriptor, message, message_length);
}

int Socket_Receive(Socket self, char * buffer, unsigned int buffer_length)
{
    int file_descriptor = 0;

    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    if (buffer == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    if (buffer_length == 0)
    {
        return SOCKET_FAIL;
    }

    file_descriptor = self->file_descriptor;

    return UnixSocket_Receive(file_descriptor, buffer, buffer_length);
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
