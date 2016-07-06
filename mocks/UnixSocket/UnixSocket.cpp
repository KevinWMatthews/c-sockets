extern "C"
{
#include "UnixSocket.h"
}

#include "CppUTestExt/MockSupport.h"

int UnixSocket_Create(void)
{
    mock().actualCall("UnixSocket_Create");
    return mock().intReturnValue();
}
