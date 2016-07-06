#include "Socket.h"
#include "UnixSocket.h"

int Socket_Open(void)
{
    return UnixSocket_Open();
}
