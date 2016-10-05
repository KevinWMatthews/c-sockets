#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

typedef struct SocketStruct * Socket;

#ifndef NULL
#define NULL 0
#endif

/*
 * If the return code == SOCKET_FAILED_SYSTEM_CALL, the underlying system call returned an error code.
 * Query errno and/or use perror to discover why the system call failed.
 * If the return code != SOCKET_FAILED_SYSTEM_CALL, the Socket wrapper layer failed.
 * Parse the return code directly; a system call may or may not have been executed.
 */
typedef enum
{
    SOCKET_INVALID_OPTION = -7,
    SOCKET_INVALID_SETTING = -6,
    SOCKET_INVALID_BUFFER = -5,
    SOCKET_ADDRESS_IN_USE = -4,
    SOCKET_ALREADY_OPEN = -3,
    SOCKET_NULL_POINTER = -2,
    SOCKET_FAILED_SYSTEM_CALL = -1,
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

typedef enum
{
    SOCKET_DOMAIN_IPV4 = 0
} SocketDomain;
typedef enum
{
    SOCKET_TYPE_STREAM = 0,
    SOCKET_TYPE_DATAGRAM = 1
} SocketType;
typedef enum
{
    SOCKET_PROTOCOL_DEFAULT = 0,
    SOCKET_PROTOCOL_TCP = 1,
    SOCKET_PROTOCOL_UDP = 2
} SocketProtocol;
typedef struct SocketSettingsStruct * SocketSettings;
typedef struct SocketSettingsStruct
{
    SocketDomain domain;
    SocketType type;
    SocketProtocol protocol;
} SocketSettingsStruct;
/*
 * Open and configure the given socket and track the file descriptor.
 * You must configure the settings when the socket is opened.
 * Other options can be set later.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int Socket_Open(Socket self, SocketSettings settings);

typedef enum
{
    SOCKET_OPTION_UDP_BROADCAST = 0,
    SOCKET_OPTION_REUSE_ADDRESS = 1
} SocketOptionName;
typedef struct SocketOptionsStruct * SocketOptions;
typedef struct SocketOptionsStruct
{
    SocketOptionName option_name;
} SocketOptionsStruct;
/*
 * Set options for the given socket.
 * Options should be set after the socket is opened (you need a file descriptor)
 * and before it is bound (options are set in place at bind).
 * There is no provision to set several options simultaneously.
 */
int Socket_SetOptions(Socket self, SocketOptions options);

/*
 * Close the given socket.
 */
void Socket_Close(Socket);

/*
 * Bind the given Socket to the given IP address and port.
 * On success, returns 0.
 * On failure, returns < 0:
 *      SOCKET_ADDRESS_IN_USE if the socket has already been bound.
 *      SOCKET_FAILED_SYSTEM_CALL on other failures.
 */
int Socket_Bind(Socket, const char * ip_address, int port);

/*
 * Mark the socket as a 'passive socket' - it will accept incoming connections if Socket_Accept() is used.
 * For connection-based sockets only.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int Socket_Listen(Socket, int backlog);

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
 * Send a message to the socket that the given socket is connected to.
 * For client sockets only.
 * Assumes that the given client socket is already connected to a server socket.
 * Message length does not include the null terminator.
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
