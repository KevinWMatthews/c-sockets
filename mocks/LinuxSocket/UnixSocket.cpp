extern "C"
{
#include "UnixSocket.h"
}

#include "CppUTestExt/MockSupport.h"

int UnixSocket_SetOption(int file_descriptor, UnixSocketOption option)
{
    mock().actualCall("UnixSocket_SetOption")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("option", option);
    return mock().intReturnValue();
}
