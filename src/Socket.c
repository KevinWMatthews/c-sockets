#include "Socket.h"
#include "SocketSystemLayer.h"

#define RETURN_IF_NULL(pointer) \
    if ( (pointer) == 0 ) { return; }

#define RETURN_VALUE_IF_NULL(pointer, value) \
    if ( (pointer) == 0 ) { return (value); }

typedef struct SocketStruct
{
    int socket_descriptor;
} SocketStruct;

Socket Socket_Create(void)
{
    Socket self = calloc( 1, sizeof(*self) );

    return self;
}

void Socket_Destroy(Socket * self)
{
    if (self == 0)
        return;
    free(*self);
    *self = 0;
}

int Socket_Open(Socket self)
{
    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);

    self->socket_descriptor = SocketSystemLayer_Open();
    if (self->socket_descriptor == SOCKET_SYSTEM_LAYER_FAIL)
    {
        self->socket_descriptor = SOCKET_INVALID_DESCRIPTOR;
        return SOCKET_FAIL;
    }
    return SOCKET_SUCCESS;
}

void Socket_Close(Socket self)
{
    RETURN_IF_NULL(self);

    SocketSystemLayer_Close(self->socket_descriptor);
    self->socket_descriptor = SOCKET_INVALID_DESCRIPTOR;
}

int Socket_GetDescriptor(Socket self)
{
    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);

    return self->socket_descriptor;
}
