#include "Socket.h"
#include "UnixSocket.h"

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
    int result = 0;
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }

    result = UnixSocket_Connect(ip_address, port);
    if ( result < 0 )
    {
        return result;
    }
    return SOCKET_SUCCESS;
}
