#ifndef LINUX_SOCKET_INCLUDED
#define LINUX_SOCKET_INCLUDED

#include "Socket.h"

typedef enum
{
    LINUX_SOCKET_FAIL = -1,
    LINUX_SOCKET_SUCCESS = 0
} LinuxSocketReturnCode;
Socket LinuxSocket_Create(void);
void LinuxSocket_Destroy(Socket *);
typedef enum
{
    LINUX_SOCKET_IPV4,
    LINUX_SOCKET_UNIX,
    LINUX_SOCKET_LOCAL
} LinuxSocketOptions;
int LinuxSocket_Open(Socket, LinuxSocketOptions);

#endif
