extern "C"
{
#include "SocketSystemLayer.h"
}

#include "CppUTestExt/MockSupport.h"

int SocketSystemLayer_Open(void)
{
    mock().actualCall("SocketSystemLayer_Open");
    return mock().intReturnValue();
}

int SocketSystemLayer_Close(int descriptor)
{
    mock().actualCall("SocketSystemLayer_Close")
        .withParameter("descriptor", descriptor);
    return mock().intReturnValue();
}

int SocketSystemLayer_Bind(int descriptor, const char * ip_address, int port)
{
    mock().actualCall("SocketSystemLayer_Bind")
        .withParameter("descriptor", descriptor)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port);
    return mock().intReturnValue();
}

int SocketSystemLayer_Listen(int descriptor, int backlog)
{
    mock().actualCall("SocketSystemLayer_Listen")
        .withParameter("descriptor", descriptor)
        .withParameter("backlog", backlog);
    return mock().intReturnValue();
}

int SocketSystemLayer_Accept(int descriptor)
{
    mock().actualCall("SocketSystemLayer_Accept")
        .withParameter("descriptor", descriptor);
    return mock().intReturnValue();
}

int SocketSystemLayer_Connect(int descriptor, const char * ip_address, int port)
{
    mock().actualCall("SocketSystemLayer_Connect")
        .withParameter("descriptor", descriptor)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port);
    return mock().intReturnValue();
}

int SocketSystemLayer_Receive(int descriptor, char * buffer, unsigned int buffer_length)
{
    mock().actualCall("SocketSystemLayer_Receive")
        .withParameter("descriptor", descriptor)
        .withParameter("buffer", buffer)
        .withParameter("buffer_length", buffer_length);
    return mock().intReturnValue();
}
