#include "Socket.h"
#include "UnixSocket.h"
#include <stdlib.h>

typedef struct SocketStruct
{
    int file_descriptor;
    const char * ip_address;
    int port;
} SocketStruct;

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
        return SOCKET_FAIL;
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

int Socket_Connect(Socket self, const char * ip_address, int port)
{
    int file_descriptor = SOCKET_FAIL, result = SOCKET_FAIL;
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    if (ip_address == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    file_descriptor = self->file_descriptor;

    result = UnixSocket_Connect(file_descriptor, ip_address, port);
    if ( result < 0 )
    {
        return result;
    }
    self->ip_address = ip_address;
    self->port = port;
    return SOCKET_SUCCESS;
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
