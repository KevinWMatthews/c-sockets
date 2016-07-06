#include "Socket.h"
#include "UnixSocket.h"

int Socket_Create(void)
{
    return UnixSocket_Create();
}
