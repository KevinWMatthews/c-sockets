#include "Socket.h"
#include "UnixSocket.h"
#include <stdlib.h>

typedef struct SocketStruct
{
    int file_descriptor;
} SocketStruct;

Socket Socket_Create(void)
{
    Socket self = calloc( 1, sizeof(*self) );
    if (self == 0)
    {
        return 0;
    }
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
    UnixSocket_Close(self->file_descriptor);
}

int Socket_GetFileDescriptor(Socket self)
{
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    return self->file_descriptor;
}

int Socket_Connect(Socket self, const char * ip_address, int port)
{
    int file_descriptor = 0, result = 0;
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }
    file_descriptor = self->file_descriptor;

    result = UnixSocket_Connect(file_descriptor, ip_address, port);
    if ( result < 0 )
    {
        return result;
    }
    return SOCKET_SUCCESS;
}

int Socket_Send(Socket self, const char * message, unsigned int message_length)
{
    int file_descriptor = 0;
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
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
    file_descriptor = self->file_descriptor;

    return UnixSocket_Receive(file_descriptor, buffer, buffer_length);
}
