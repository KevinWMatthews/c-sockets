#ifndef UBUNTU_SYSTEM_INCLUDED
#define UBUNTU_SYSTEM_INCLUDED

typedef enum
{
    UBUNTU_SYSTEM_UNIX_SOCKET
} UbuntuSocketType;
int UbuntuSystem_Open(UbuntuSocketType);

#endif
