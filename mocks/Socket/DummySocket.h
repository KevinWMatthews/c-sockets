#ifndef DUMMY_SOCKET_INCLUDED
#define DUMMY_SOCKET_INCLUDED

#include "Socket.h"

Socket DummySocket_Create(void);
typedef enum
{
    DUMMY_SOCKET_FAIL = -1,
    DUMMY_SOCKET_SUCCESS = 0
} DummySocketReturnCode;
int DummySocket_Open(void);

#endif
