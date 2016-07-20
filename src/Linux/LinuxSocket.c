#include "LinuxSocket.h"
#include "LinuxSocketOs.h"

Socket LinuxSocket_Create(void)
{
    return 0;
}

void LinuxSocket_Destroy(Socket * self)
{
}

int LinuxSocket_Open(Socket self, LinuxSocketOptions options)
{
    int result = LINUX_SOCKET_FAIL;
    result = LinuxSocketOs_Open(options);
    return result;
}
