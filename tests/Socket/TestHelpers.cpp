#include "TestHelpers.h"
#include "CppUTestExt/MockSupport.h"

// All sockets
void expectSocketOpen(int socket_descriptor_or_error_code, int domain, int type, int protocol)
{
    mock().expectOneCall("SocketSystemLayer_Open")
        .withParameter("domain", domain)
        .withParameter("type", type)
        .withParameter("protocol", protocol)
        .andReturnValue(socket_descriptor_or_error_code);
}

void expectSocketClose(int socket_descriptor, int return_code)
{
    mock().expectOneCall("SocketSystemLayer_Close")
        .withParameter("descriptor", socket_descriptor)
        .andReturnValue(return_code);
}

void expectSetOption(int socket_descriptor, int option_level, int option_name, int return_code)
{
    int option_value = 1;
    mock().expectOneCall("SocketSystemLayer_SetOptions")
        .withParameter("descriptor", socket_descriptor)
        .withParameter("option_level", option_level)
        .withParameter("option_name", option_name)
        .withParameter("option_value", option_value)
        .withParameter("option_length", sizeof(option_value))
        .andReturnValue(return_code);
}

void expectSocketReceive(int socket_descriptor, char * buffer, unsigned int buffer_length, int return_code)
{
    mock().expectOneCall("SocketSystemLayer_Receive")
        .withParameter("descriptor", socket_descriptor)
        .withParameter("buffer", buffer)
        .withParameter("buffer_length", buffer_length)
        .andReturnValue(return_code);
}

void expectSocketSend(int socket_descriptor, char * message, unsigned int message_length, int return_code)
{
    mock().expectOneCall("SocketSystemLayer_Send")
        .withParameter("descriptor", socket_descriptor)
        .withParameter("message", message)
        .withParameter("message_length", message_length)
        .andReturnValue(return_code);
}

void expectSocketSendTo(int socket_descriptor, char * message, unsigned int message_length, const char * ip_address, int port, int return_code)
{
    mock().expectOneCall("SocketSystemLayer_Send")
        .withParameter("descriptor", socket_descriptor)
        .withParameter("message", message)
        .withParameter("message_length", message_length)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port)
        .andReturnValue(return_code);
}

// Client sockets
void expectSocketConnect(int socket_descriptor, int domain, const char *ip_address, int port, int return_code)
{
    mock().expectOneCall("SocketSystemLayer_Connect")
        .withParameter("domain", domain)
        .withParameter("descriptor", socket_descriptor)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port)
        .andReturnValue(return_code);
}

// Server sockets
void expectSocketBind(int socket_descriptor, int domain, SocketAddress socket_address, int return_code)
{
    mock().expectOneCall("SocketSystemLayer_Bind")
        .withParameter("domain", domain)
        .withParameter("descriptor", socket_descriptor)
        .withParameter("ip_address", socket_address->ip_address)
        .withParameter("port", socket_address->port)
        .andReturnValue(return_code);
}

void expectSocketListen(int socket_descriptor, int backlog, int return_code)
{
    mock().expectOneCall("SocketSystemLayer_Listen")
        .withParameter("descriptor", socket_descriptor)
        .withParameter("backlog", backlog)
        .andReturnValue(return_code);
}

void expectSocketAccept(int server_socket_descriptor, int client_socket_descriptor)
{
    mock().expectOneCall("SocketSystemLayer_Accept")
        .withParameter("descriptor", server_socket_descriptor)
        .andReturnValue(client_socket_descriptor);
}
