#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "UbuntuSystem.h"
}

int UbuntuSystem_Open(UbuntuSocketType socket_type)
{
    mock().actualCall("UbuntuSystem_Open")
        .withParameter("socket_type", socket_type);
    return mock().intReturnValue();
}
