#ifndef LINUX_SOCKET_OS_INCLUDED
#define LINUX_SOCKET_OS_INCLUDED

typedef enum
{
    LINUX_SOCKET_OS_FAIL = -1,
    LINUX_SOCKET_OS_SUCCESS = 0
} LinuxSocketOsReturnCode;
typedef enum
{
    LINUX_SOCKET_OS_AF_INET,
    LINUX_SOCKET_OS_AF_UNIX,
    LINUX_SOCKET_OS_AF_LOCAL
} LinuxSocketOsOptions;
int LinuxSocketOs_Open(int domain);

#endif
