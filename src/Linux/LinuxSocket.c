#include "LinuxSocket.h"
#include "UnixSocket.h" //TODO change this to UbuntuSystem
#include "UbuntuSystem.h"
#include <stdlib.h>

typedef struct LinuxSocketStruct
{
    SocketStruct base;
} LinuxSocketStruct;

static SocketInterfaceStruct interface_struct;
static SocketInterface interface = &interface_struct;

Socket LinuxSocket_Create(void)
{
    LinuxSocket self = calloc( 1, sizeof(*self) );
    if (self == 0)
    {
        return 0;
    }
    self->base.interface = interface;
    return (Socket)self;
}

void LinuxSocket_Destroy(Socket * self)
{
    if (self == 0)
        return;
    free(*self);
    *self = 0;
}

int LinuxSocket_SetOption(Socket self, LinuxSocketOption option)
{
    UnixSocketOption unix_option = 0;
    if (self == 0)
    {
        return SOCKET_NULL_POINTER;
    }

    switch (option)
    {
        case LINUX_SOCKET_IMMEDIATELY_REUSE_SOCKET:
            unix_option = UNIX_SOCKET_IMMEDIATELY_REUSE_SOCKET;
    }

    return UnixSocket_SetOption(self->file_descriptor, unix_option);
}

int LinuxSocket_Open(Socket self, LinuxSocketOptions options)
{
    return UbuntuSystem_Open(UBUNTU_SYSTEM_UNIX_SOCKET);
}
