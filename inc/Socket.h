#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

typedef struct SocketStruct * Socket;

// Returns a pointer to a socket on success.
// Returns 0 if a Socket can not be created.
Socket Socket_Create(void);
void Socket_Destroy(Socket *);

typedef enum
{
    SOCKET_INVALID_BUFFER = -5,
    SOCKET_ADDRESS_IN_USE = -4,
    SOCKET_ALREADY_OPEN = -3,
    SOCKET_NULL_POINTER = -2,
    SOCKET_FAIL = -1,
    SOCKET_SUCCESS = 0
} SocketReturnCode;
int Socket_Open(Socket);
void Socket_Close(Socket);
int Socket_Bind(Socket, const char * ip_address, int port);
int Socket_Listen(Socket);

// Pass a server socket that is going to accept a connection.
// On success, returns a pointer to the client socket that it connected to.
// On failure, returns a null pointer.
Socket Socket_Accept(Socket);

/*
 * Pass a client socket and the IP address and port that you
 * wish to connect to.
 * Returns SOCKET_RETURN_CODE
 */
int Socket_Connect(Socket, const char * ip_address, int port);

enum
{
    SOCKET_INVALID_DESCRIPTOR = -1,
};
int Socket_GetDescriptor(Socket);

/*
 * Receive a message from the given socket.
 * Copy this message into the buffer.
 * You must specify the buffer length (without null terminator).
 * On success, returns the length of the message received.
 * On failure, returns < 0.
 */
int Socket_Receive(Socket, char * buffer, unsigned int buffer_length);

/*
 * Send a message to the given socket.
 * Messge length does not include the null terminator.
 * On success, returns the number of characters in the message that was sent (not including null terminator).
 * On failure, returns < 0.
 */
int Socket_Send(Socket, char * message, unsigned int message_length);

enum
{
    SOCKET_INVALID_IP_ADDRESS = 0
};
const char * Socket_GetIpAddress(Socket);
enum
{
    SOCKET_INVALID_PORT = -1
};
int Socket_GetPort(Socket);

#endif
