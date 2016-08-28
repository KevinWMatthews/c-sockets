#include "SocketHelpers.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

void expectSocketOpen(int file_descriptor_or_error_code)
{
    mock().expectOneCall("UnixSocket_Open")
        .andReturnValue(file_descriptor_or_error_code);
}

void expectSocketClose(int file_descriptor)
{
    mock().expectOneCall("UnixSocket_Close")
        .withParameter("file_descriptor", file_descriptor);
}

void expectSocketConnect(int file_descriptor, const char * ip_address, int port, int result)
{
    mock().expectOneCall("UnixSocket_Connect")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port)
        .andReturnValue(result);
}

void expectSocketBind(int file_descriptor, const char * ip_address, int port, int result)
{
    mock().expectOneCall("UnixSocket_Bind")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port)
        .andReturnValue(result);
}

void expectSocketSend(int file_descriptor, const char * message, unsigned int message_length, int result)
{
    mock().expectOneCall("UnixSocket_Send")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("message", message)
        .withParameter("message_length", message_length)
        .andReturnValue(result);
}

void expectSocketReceive(int file_descriptor, char * buffer, unsigned int buffer_length, int result)
{
    mock().expectOneCall("UnixSocket_Receive")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("buffer", buffer)
        .withParameter("buffer_length", buffer_length)
        .andReturnValue(result);
}

void expectSocketListen(int file_descriptor, int backlog, int result)
{
    mock().expectOneCall("UnixSocket_Listen")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("backlog", backlog)
        .andReturnValue(result);
}

void expectSocketAccept(int file_descriptor, int result)
{
    mock().expectOneCall("UnixSocket_Accept")
        .withParameter("file_descriptor", file_descriptor)
        .andReturnValue(result);
}

void expectSocketSetOption(int file_descriptor, int option, int result)
{
    mock().expectOneCall("UnixSocket_SetOption")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("option", option)
        .andReturnValue(result);
}
