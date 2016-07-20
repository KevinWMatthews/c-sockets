extern "C"
{
#include "LinuxSocketOs.h"
}

#include "CppUTestExt/MockSupport.h"

int LinuxSocketOs_Open(int domain)
{
    mock().actualCall("LinuxSocketOs_Open")
        .withParameter("domain", domain);
    return mock().intReturnValue();
}
