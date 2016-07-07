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

int UnixSocket_Connect(const char * ip_address, int port)
{
    mock().actualCall("UnixSocket_Connect")
        .withParameter("ip_address", ip_address)
        .withParameter("port", port);
    return mock().intReturnValue();
}
