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
