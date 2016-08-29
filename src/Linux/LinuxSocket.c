#include "LinuxSocket.h"
#include <stdlib.h>

typedef struct LinuxSocketStruct * LinuxSocket;
typedef struct LinuxSocketStruct
{
    int make_compiler_happy;
} LinuxSocketStruct;

Socket LinuxSocket_Create(void)
{
    LinuxSocket self = calloc( 1, sizeof(*self) );
    return (Socket)self;
}

void LinuxSocket_Destroy(Socket * self)
{
    if (self == 0)
        return;
    free(*self);
    *self = 0;
}

// int LinuxSocket_Open(Socket self, LinuxSocketOptions options)
// {
//     int result = LINUX_SOCKET_FAIL;

//     if (self == 0)
//     {
//         return LINUX_SOCKET_FAIL;
//     }
//     result = LinuxSocketOs_Open(options);
//     if (result >= 0)
//     {
//         return LINUX_SOCKET_SUCCESS;
//     }
//     return LINUX_SOCKET_FAIL;
// }
