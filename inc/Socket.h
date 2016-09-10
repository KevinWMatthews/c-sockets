#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

typedef struct SocketStruct * Socket;

Socket Socket_Create(void);
void Socket_Destroy(Socket);

#endif
