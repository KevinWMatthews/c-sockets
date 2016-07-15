extern "C"
{
#include "UnixSocket.h"
}

#include "CppUTestExt/MockSupport.h"

int UnixSocket_Open(void)
{
    mock().actualCall("UnixSocket_Open");
    return mock().intReturnValue();
}

void UnixSocket_Close(int file_descriptor)
{
    mock().actualCall("UnixSocket_Close")
        .withParameter("file_descriptor", file_descriptor);
}

int UnixSocket_Bind(int file_descriptor, const char * ip_address, int port)
{
    mock().actualCall("UnixSocket_Bind")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port);
    return mock().intReturnValue();
}

int UnixSocket_Connect(int file_descriptor, const char * ip_address, int port)
{
    mock().actualCall("UnixSocket_Connect")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port);
    return mock().intReturnValue();
}

int UnixSocket_Send(int file_descriptor, const char * message, unsigned int message_length)
{
    mock().actualCall("UnixSocket_Send")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("message", message)
        .withParameter("message_length", message_length);
    return mock().intReturnValue();
}

int UnixSocket_Receive(int file_descriptor, char * buffer, unsigned int buffer_length)
{
    mock().actualCall("UnixSocket_Receive")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("buffer", buffer)
        .withParameter("buffer_length", buffer_length);
    return mock().intReturnValue();
}

int UnixSocket_Listen(int file_descriptor, int backlog)
{
    mock().actualCall("UnixSocket_Listen")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("backlog", backlog);
    return mock().intReturnValue();
}

int UnixSocket_Accept(int file_descriptor)
{
    mock().actualCall("UnixSocket_Accept")
        .withParameter("file_descriptor", file_descriptor);
    return mock().intReturnValue();
}

int UnixSocket_SetOption(int file_descriptor, UnixSocketOption option)
{
    mock().actualCall("UnixSocket_SetOption")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("option", option);
    return mock().intReturnValue();
}
