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
