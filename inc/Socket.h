#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

typedef struct SocketStruct * Socket;

typedef enum
{
    SOCKET_INVALID_BUFFER = -5,
    SOCKET_ADDRESS_IN_USE = -4,
    SOCKET_ALREADY_OPEN = -3,
    SOCKET_NULL_POINTER = -2,
    SOCKET_FAIL = -1,
    SOCKET_SUCCESS = 0
} SocketReturnCode;

/*
 * On success, returns a pointer to a new Socket.
 * On failure, returns a null pointer.
 */
Socket Socket_Create(void);

/*
 * Frees all dynamically allocated memory and sets the pointer to null.
 */
void Socket_Destroy(Socket *);

/*
 * Open the given socket and track the file descriptor.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int Socket_Open(Socket);

/*
 * Close the given socket.
 */
void Socket_Close(Socket);

/*
 * Bind the given Socket to the given IP address and port.
 * On success, returns 0.
 * On failure, returns < 0:
 *      SOCKET_ADDRESS_IN_USE if the socket has already been bound.
 *      SOCKET_FAIL on other failures.
 */
int Socket_Bind(Socket, const char * ip_address, int port);

/*
 * Mark the socket as a 'passive socket' - it will accept incoming connections if Socket_Accept() is used.
 * For connection-based sockets only.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int Socket_Listen(Socket);

/*
 * Command the given server Socket to connect to the first connection request.
 * For connection-based sockets only.
 * The socket must first be told to 'listen' using Socket_Listen().
 * On success, returns a pointer to the new client socket that the original server socket connected to.
 * On failure, returns a null pointer.
 */
Socket Socket_Accept(Socket);

/*
 * Connect the given client Socket to a server at the the specified IP address and port.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int Socket_Connect(Socket, const char * ip_address, int port);

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

/*
 * Return the file descriptor for the given Socket.
 */
enum
{
    SOCKET_INVALID_DESCRIPTOR = -1,
};
int Socket_GetDescriptor(Socket);

/*
 * Return the IP address of the given Socket.
 */
enum
{
    SOCKET_INVALID_IP_ADDRESS = 0
};
const char * Socket_GetIpAddress(Socket);

/*
 * Return the port of the given Socket.
 */
enum
{
    SOCKET_INVALID_PORT = -1
};
int Socket_GetPort(Socket);

#endif
