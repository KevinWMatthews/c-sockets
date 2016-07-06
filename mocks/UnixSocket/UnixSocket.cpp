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
