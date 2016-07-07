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
