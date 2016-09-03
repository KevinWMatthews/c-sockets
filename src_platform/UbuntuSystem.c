#include "UbuntuSystem.h"
#include <sys/types.h>
#include <sys/socket.h>

int UbuntuSystem_Open(UbuntuSocketType socket_type)
{
    return socket(AF_INET, SOCK_STREAM, 0);
}
