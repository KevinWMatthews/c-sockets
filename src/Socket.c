#include "Socket.h"
#include "UnixSocket.h"

int Socket_Create(void)
{
    UnixSocket_Create();
    return SOCKET_FAIL;
}
