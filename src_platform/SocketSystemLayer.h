#ifndef SOCKET_SYSTEM_LAYER_INCLUDED
#define SOCKET_SYSTEM_LAYER_INCLUDED

#define SOCKET_SYSTEM_LAYER_FAIL    -1
#define SOCKET_SYSTEM_LAYER_SUCCESS  0
int SocketSystemLayer_Open(void);
int SocketSystemLayer_Close(int);
int SocketSystemLayer_Bind(int descriptor, const char * ip_address, int port);

#endif
