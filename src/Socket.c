#include "Socket.h"
#include "SocketSystemLayer.h"

#define RETURN_IF_NULL(pointer) \
    if ( (pointer) == 0 ) { return; }

#define RETURN_VALUE_IF_NULL(pointer, value) \
    if ( (pointer) == 0 ) { return (value); }

typedef struct SocketStruct
{
    int socket_descriptor;
    const char * ip_address;
    int port;
} SocketStruct;

Socket Socket_Create(void)
{
    Socket self = calloc( 1, sizeof(*self) );
    RETURN_VALUE_IF_NULL(self, 0);

    self->socket_descriptor = SOCKET_INVALID_DESCRIPTOR;
    self->ip_address = SOCKET_INVALID_IP_ADDRESS;
    self->port = SOCKET_INVALID_PORT;
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
    if (self->socket_descriptor != SOCKET_INVALID_DESCRIPTOR)
        return SOCKET_ALREADY_OPEN;

    self->socket_descriptor = SocketSystemLayer_Open();
    if (self->socket_descriptor == SOCKET_SYSTEM_LAYER_FAIL)
    {
        return SOCKET_FAIL;
    }
    return SOCKET_SUCCESS;
}

void Socket_Close(Socket self)
{
    RETURN_IF_NULL(self);
    if (self->socket_descriptor == SOCKET_INVALID_DESCRIPTOR)
        return;

    SocketSystemLayer_Close(self->socket_descriptor);
    self->socket_descriptor = SOCKET_INVALID_DESCRIPTOR;
    self->ip_address = SOCKET_INVALID_IP_ADDRESS;
    self->port = SOCKET_INVALID_PORT;
}

int Socket_Bind(Socket self, const char * ip_address, int port)
{
    int return_code = SOCKET_SYSTEM_LAYER_FAIL;
    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);
    return_code = SocketSystemLayer_Bind(self->socket_descriptor, ip_address, port);
    if ( return_code < 0 )
    {
        return SOCKET_FAIL;
    }
    self->ip_address = ip_address;
    self->port = port;
    return SOCKET_SUCCESS;
}

int Socket_GetDescriptor(Socket self)
{
    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);

    return self->socket_descriptor;
}

const char * Socket_GetIpAddress(Socket self)
{
    RETURN_VALUE_IF_NULL(self, SOCKET_INVALID_IP_ADDRESS);
    return self->ip_address;
}

int Socket_GetPort(Socket self)
{
    RETURN_VALUE_IF_NULL(self, SOCKET_INVALID_PORT);
    return self->port;
}
