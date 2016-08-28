#ifndef SOCKET_HELPERS_INCLUDED
#define SOCKET_HELPERS_INCLUDED

void expectSocketOpen(int file_descriptor_or_error_code);
void expectSocketClose(int file_descriptor);
void expectSocketConnect(int file_descriptor, const char * ip_address, int port, int result);
void expectSocketBind(int file_descriptor, const char * ip_address, int port, int result);
void expectSocketSend(int file_descriptor, const char * message, unsigned int message_length, int result);
void expectSocketReceive(int file_descriptor, char * buffer, unsigned int buffer_length, int result);
void expectSocketListen(int file_descriptor, int backlog, int result);
void expectSocketAccept(int file_descriptor, int result);
void expectSocketSetOption(int file_descriptor, int option, int result);

#endif
