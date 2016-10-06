#include "Socket.h"
#include "SocketSystemLayer.h"
#include <stdlib.h>
#include <stdio.h>

#define RETURN_IF_NULL(pointer) \
    if ( (pointer) == 0 ) { return; }

#define RETURN_VALUE_IF_NULL(pointer, value) \
    if ( (pointer) == 0 ) { return (value); }

typedef struct SocketStruct
{
    int socket_descriptor;
    SocketSystemLayerDomain domain; // Also referred to as family.
    const char * ip_address;
    int port;
} SocketStruct;

Socket Socket_Create(void)
{
    Socket self = calloc( 1, sizeof(*self) );
    RETURN_VALUE_IF_NULL(self, NULL);

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

static int convert_socket_domain(SocketDomain domain)
{
    switch (domain)
    {
        case SOCKET_DOMAIN_IPV4:
            return SOCKET_SYSTEM_DOMAIN_IPV4;
    }
    return SOCKET_INVALID_SETTING;
}

static int convert_socket_type(SocketType type)
{
    switch (type)
    {
        case SOCKET_TYPE_STREAM:
            return SOCKET_SYSTEM_TYPE_STREAM;
        case SOCKET_TYPE_DATAGRAM:
            return SOCKET_SYSTEM_TYPE_DATAGRAM;
    }
    return SOCKET_INVALID_SETTING;
}

static int convert_socket_protocol(SocketProtocol protocol)
{
    switch (protocol)
    {
        case SOCKET_PROTOCOL_DEFAULT:
            return SOCKET_SYSTEM_PROTOCOL_DEFAULT;
        case SOCKET_PROTOCOL_TCP:
            return SOCKET_SYSTEM_PROTOCOL_TCP;
        case SOCKET_PROTOCOL_UDP:
            return SOCKET_SYSTEM_PROTOCOL_UDP;
    }
    return SOCKET_INVALID_SETTING;
}

int Socket_Open(Socket self, SocketSettings settings)
{
    int system_domain = -1;
    int system_type = -1;
    int system_protocol = -1;

    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);
    RETURN_VALUE_IF_NULL(settings, SOCKET_NULL_POINTER);
    if (self->socket_descriptor != SOCKET_INVALID_DESCRIPTOR)
        return SOCKET_ALREADY_OPEN;

    system_domain = convert_socket_domain(settings->domain);
    if (system_domain == SOCKET_INVALID_SETTING)
        return SOCKET_INVALID_SETTING;
    self->domain = system_domain;

    system_type = convert_socket_type(settings->type);
    if (system_type == SOCKET_INVALID_SETTING)
        return SOCKET_INVALID_SETTING;

    system_protocol = convert_socket_protocol(settings->protocol);
    if (system_protocol == SOCKET_INVALID_SETTING)
        return SOCKET_INVALID_SETTING;

    self->socket_descriptor = SocketSystemLayer_Open(system_domain, system_type, system_protocol);
    if (self->socket_descriptor < 0)
        return SOCKET_FAILED_SYSTEM_CALL;
    return SOCKET_SUCCESS;
}

int Socket_SetOptions(Socket self, SocketOptions options)
{
    const int option_value = 1;
    int option_name = -1;
    int return_code = SOCKET_SYSTEM_LAYER_FAIL;

    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);
    RETURN_VALUE_IF_NULL(options, SOCKET_NULL_POINTER);

    switch (options->option_name)
    {
        case SOCKET_OPTION_UDP_BROADCAST:
            option_name = SOCKET_SYSTEM_OPTION_BROADCAST;
            break;
        case SOCKET_OPTION_REUSE_ADDRESS:
            option_name = SOCKET_SYSTEM_OPTION_REUSE_ADDRESS;
            break;
        default:
            return SOCKET_INVALID_OPTION;
    }
    return_code = SocketSystemLayer_SetOptions(
            self->socket_descriptor,
            SOCKET_SYSTEM_OPTION_LEVEL_SOCKET,
            option_name,
            (void *)&option_value,
            sizeof(int) );
    if (return_code < 0)
        return SOCKET_FAILED_SYSTEM_CALL;
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

int SocketServer_Bind(Socket self, const char * ip_address, int port)
{
    int return_code = SOCKET_SYSTEM_LAYER_FAIL;
    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);
    return_code = SocketSystemLayer_Bind(self->socket_descriptor, self->domain, ip_address, port);
    if ( return_code == SOCKET_SYSTEM_LAYER_ADDRESS_IN_USE )
    {
        return SOCKET_ADDRESS_IN_USE;
    }
    else if ( return_code < 0 )
    {
        return SOCKET_FAILED_SYSTEM_CALL;
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

int SocketServer_Listen(Socket self, int backlog)
{
    int return_code = SOCKET_SYSTEM_LAYER_FAIL;
    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);

    return_code = SocketSystemLayer_Listen(self->socket_descriptor, backlog);
    if ( return_code < 0 )
        return SOCKET_FAILED_SYSTEM_CALL;
    return SOCKET_SUCCESS;
}

Socket SocketServer_Accept(Socket self)
{
    Socket client_socket = {0};
    int return_code = SOCKET_SYSTEM_LAYER_FAIL;
    RETURN_VALUE_IF_NULL(self, NULL);

    return_code = SocketSystemLayer_Accept(self->socket_descriptor);
    if ( return_code < 0 )
        return NULL;

    client_socket = Socket_Create();
    RETURN_VALUE_IF_NULL(client_socket, NULL);
    client_socket->socket_descriptor = return_code;
    return client_socket;
}

int SocketClient_Connect(Socket self, const char * ip_address, int port)
{
    int return_code = SOCKET_SYSTEM_LAYER_FAIL;
    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);
    RETURN_VALUE_IF_NULL(ip_address, SOCKET_NULL_POINTER);

    return_code = SocketSystemLayer_Connect(self->socket_descriptor, self->domain, ip_address, port);
    if (return_code < 0)
        return SOCKET_FAILED_SYSTEM_CALL;
    return SOCKET_SUCCESS;
}

int Socket_Receive(Socket self, char * buffer, unsigned int buffer_length)
{
    int return_code = SOCKET_SYSTEM_LAYER_FAIL;
    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);
    RETURN_VALUE_IF_NULL(buffer, SOCKET_INVALID_BUFFER);
    if (buffer_length == 0)
        return SOCKET_INVALID_BUFFER;

    return_code = SocketSystemLayer_Receive(self->socket_descriptor, buffer, buffer_length);
    if (return_code < 0)
        return SOCKET_FAILED_SYSTEM_CALL;
    return return_code;
}

int Socket_Send(Socket self, char * message, unsigned int message_length)
{
    int return_code = SOCKET_SYSTEM_LAYER_FAIL;
    RETURN_VALUE_IF_NULL(message, SOCKET_INVALID_BUFFER);
    if (message_length == 0)
        return SOCKET_INVALID_BUFFER;

    return_code = SocketSystemLayer_Send(self->socket_descriptor, message, message_length);
    if (return_code < 0)
        return SOCKET_FAILED_SYSTEM_CALL;
    return return_code;
}

int Socket_SendTo(Socket self, char * message, unsigned int message_length,
        const char * ip_address, int port)
{
    int return_code = 0;
    RETURN_VALUE_IF_NULL(self, SOCKET_NULL_POINTER);
    RETURN_VALUE_IF_NULL(message, SOCKET_INVALID_BUFFER);

    return_code = SocketSystemLayer_SendTo(self->socket_descriptor, message, message_length, ip_address, port);
    if (return_code < 0)
        return SOCKET_FAILED_SYSTEM_CALL;
    return return_code;
}
