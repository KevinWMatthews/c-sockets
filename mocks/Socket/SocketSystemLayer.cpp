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
