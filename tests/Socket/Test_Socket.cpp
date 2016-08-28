extern "C"
{
#include "Socket.h"
#include "UnixSocket.h"
}

#include "Test_Socket.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(Socket)
{
    int file_descriptor;
    int file_descriptor2;
    Socket socket;
    Socket socket2;

    void setup()
    {
        mock().strictOrder();
        file_descriptor = 42;
        file_descriptor2 = 43;
        socket = Socket_Create();
        socket2 = Socket_Create();
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        Socket_Destroy(&socket2);
        mock().checkExpectations();
        mock().clear();
    }

    void expectSocketOpen(int file_descriptor_or_error_code)
    {
        mock().expectOneCall("UnixSocket_Open")
            .andReturnValue(file_descriptor_or_error_code);
    }

    void expectSocketClose(int file_descriptor)
    {
        mock().expectOneCall("UnixSocket_Close")
            .withParameter("file_descriptor", file_descriptor);
    }

    void expectSocketConnect(int file_descriptor, const char * ip_address, int port, int result)
    {
        mock().expectOneCall("UnixSocket_Connect")
            .withParameter("file_descriptor", file_descriptor)
            .withParameter("ip_address", ip_address)
            .withParameter("port", port)
            .andReturnValue(result);
    }

    void expectSocketBind(int file_descriptor, const char * ip_address, int port, int result)
    {
        mock().expectOneCall("UnixSocket_Bind")
            .withParameter("file_descriptor", file_descriptor)
            .withParameter("ip_address", ip_address)
            .withParameter("port", port)
            .andReturnValue(result);
    }

    void expectSocketSend(int file_descriptor, const char * message, unsigned int message_length, int result)
    {
        mock().expectOneCall("UnixSocket_Send")
            .withParameter("file_descriptor", file_descriptor)
            .withParameter("message", message)
            .withParameter("message_length", message_length)
            .andReturnValue(result);
    }

    void expectSocketReceive(int file_descriptor, char * buffer, unsigned int buffer_length, int result)
    {
        mock().expectOneCall("UnixSocket_Receive")
            .withParameter("file_descriptor", file_descriptor)
            .withParameter("buffer", buffer)
            .withParameter("buffer_length", buffer_length)
            .andReturnValue(result);
    }

    void expectSocketListen(int file_descriptor, int backlog, int result)
    {
        mock().expectOneCall("UnixSocket_Listen")
            .withParameter("file_descriptor", file_descriptor)
            .withParameter("backlog", backlog)
            .andReturnValue(result);
    }

    void expectSocketAccept(int file_descriptor, int result)
    {
        mock().expectOneCall("UnixSocket_Accept")
            .withParameter("file_descriptor", file_descriptor)
            .andReturnValue(result);
    }

    void expectSocketSetOption(int file_descriptor, int option, int result)
    {
        mock().expectOneCall("UnixSocket_SetOption")
            .withParameter("file_descriptor", file_descriptor)
            .withParameter("option", option)
            .andReturnValue(result);
    }
};

/* Test List:
 *  SetOption:
 *
 *  GetIpAddress:
 *
 *  GetPort:
 *
 *  Accept:
 *      Null address - is valid, I think.
 *      Null size pointer - is valid if address is null.
 *      Should we verify the contents of the client address and client address length?
 *
 *  Connect:
 *      Error if passed invalid IP address.
 *      Error if passed invalid port.
 *
 *  Bind:
 *      Error if passed invalid IP address.
 *      Error if passed invalid port.
 *
 *  Send:
 *
 *  Receive:
 */

TEST(Socket, it_can_create_destroy_a_socket)
{
    // Create happens in setup
    // Destroy happens in teardown
}

TEST(Socket, it_can_create_and_double_destroy_a_socket_struct)
{
    Socket_Destroy(&socket);
    POINTERS_EQUAL(NULL, socket);
    // Destroy happens again in teardown
}

TEST(Socket, it_can_handle_null_pointers)
{
    char buffer[10] = {0};
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_GetFileDescriptor(NULL) );
    POINTERS_EQUAL( NULL, Socket_GetIpAddress(NULL) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_GetPort(NULL) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Open(NULL) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_SetOption(NULL, SOCKET_IMMEDIATELY_REUSE_SOCKET) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Bind(NULL, "0.0.0.0", 0) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Connect(NULL, "0.0.0.0", 0) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Send(NULL, "msg", 3) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Receive(NULL, buffer, 10) );
    Socket_Close(NULL);
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Listen(NULL, 10) );
    POINTERS_EQUAL( NULL, Socket_Accept(NULL) );
}

// Open
TEST(Socket, it_can_fail_to_open_a_socket)
{
    expectSocketOpen(UNIX_SOCKET_FAIL);
    LONGS_EQUAL( SOCKET_FAIL, Socket_Open(socket) );
    LONGS_EQUAL( SOCKET_INVALID_FILE_DESCRIPTOR, Socket_GetFileDescriptor(socket) );
}

TEST(Socket, it_can_open_a_socket)
{
    expectSocketOpen(file_descriptor);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open(socket) );
    LONGS_EQUAL( file_descriptor, Socket_GetFileDescriptor(socket) );
}

// Close
TEST(Socket, it_can_close_a_socket)
{
    expectSocketOpen(file_descriptor);
    expectSocketClose(file_descriptor);
    Socket_Open(socket);

    Socket_Close(socket);
    LONGS_EQUAL( SOCKET_INVALID_FILE_DESCRIPTOR, Socket_GetFileDescriptor(socket) );
}

TEST(Socket, it_can_open_several_sockets)
{
    expectSocketOpen(file_descriptor);
    expectSocketOpen(file_descriptor2);

    Socket_Open(socket);
    Socket_Open(socket2);
    LONGS_EQUAL( file_descriptor, Socket_GetFileDescriptor(socket) );
    LONGS_EQUAL( file_descriptor2, Socket_GetFileDescriptor(socket2) );
}

TEST(Socket, it_can_close_several_sockets)
{
    expectSocketOpen(file_descriptor);
    expectSocketOpen(file_descriptor2);
    expectSocketClose(file_descriptor);
    expectSocketClose(file_descriptor2);
    Socket_Open(socket);
    Socket_Open(socket2);

    Socket_Close(socket);
    Socket_Close(socket2);
    LONGS_EQUAL( SOCKET_INVALID_FILE_DESCRIPTOR, Socket_GetFileDescriptor(socket) );
    LONGS_EQUAL( SOCKET_INVALID_FILE_DESCRIPTOR, Socket_GetFileDescriptor(socket2) );
}

// Set options
TEST(Socket, it_can_fail_to_set_a_socket_option)
{
    expectSocketOpen(file_descriptor);
    expectSocketSetOption(file_descriptor, UNIX_SOCKET_IMMEDIATELY_REUSE_SOCKET, UNIX_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    LONGS_EQUAL( SOCKET_FAIL, Socket_SetOption(socket, SOCKET_IMMEDIATELY_REUSE_SOCKET) );
    Socket_Close(socket);
}

TEST(Socket, it_can_set_socket_option_immediate_reuse)
{
    expectSocketOpen(file_descriptor);
    expectSocketSetOption(file_descriptor, UNIX_SOCKET_IMMEDIATELY_REUSE_SOCKET, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    LONGS_EQUAL( SOCKET_SUCCESS, Socket_SetOption(socket, SOCKET_IMMEDIATELY_REUSE_SOCKET) );
    Socket_Close(socket);
}

// Connect
TEST(Socket, it_can_fail_to_connect_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Connect(socket, ip_address, port) );

    Socket_Close(socket);
}

TEST(Socket, it_does_not_allow_a_null_ip_address)
{
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Connect(socket, NULL, port) );
    Socket_Close(socket);
}

TEST(Socket, it_can_connect_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Connect(socket, ip_address, port) );
    STRCMP_EQUAL( ip_address, Socket_GetIpAddress(socket) );
    LONGS_EQUAL( port, Socket_GetPort(socket) );

    Socket_Close(socket);
}

// Bind
TEST(Socket, it_can_fail_to_bind_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, UNIX_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Bind(socket, ip_address, port) );
    POINTERS_EQUAL( NULL, Socket_GetIpAddress(socket) );
    LONGS_EQUAL( SOCKET_INVALID_PORT, Socket_GetPort(socket) );

    Socket_Close(socket);
}

TEST(Socket, it_will_bind_to_any_ip_address_with_a_null_ip_address_pointer)
{
    const char * ip_address = NULL;
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Bind(socket, ip_address, port) );

    Socket_Close(socket);
}

TEST(Socket, it_can_to_bind_to_a_specific_ip_and_port)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Bind(socket, ip_address, port) );
    STRCMP_EQUAL( ip_address, Socket_GetIpAddress(socket) );
    LONGS_EQUAL( port, Socket_GetPort(socket) );

    Socket_Close(socket);
}

// Get IP addrss and port
TEST(Socket, it_has_null_address_and_port_after_init)
{
    POINTERS_EQUAL( NULL, Socket_GetIpAddress(socket) );
    LONGS_EQUAL( SOCKET_INVALID_PORT, Socket_GetPort(socket) );
}

TEST(Socket, it_has_no_ip_address_and_port_after_closing_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);

    Socket_Close(socket);

    STRCMP_EQUAL( NULL, Socket_GetIpAddress(socket) );
    LONGS_EQUAL( SOCKET_INVALID_PORT, Socket_GetPort(socket) );
}

// Send
TEST(Socket, it_can_fail_send_data_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char message[] = "Hello";
    unsigned int message_length = sizeof(message);

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketSend(file_descriptor, message, message_length, UNIX_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Send(socket, message, message_length) );

    Socket_Close(socket);
}

TEST(Socket, it_will_not_send_a_null_message)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char message[] = "Hello";
    unsigned int message_length = sizeof(message);

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Send(socket, NULL, message_length) );

    Socket_Close(socket);
}

TEST(Socket, it_will_not_send_a_zero_length_message)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char message[] = "Hello";
    unsigned int message_length = 0;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Send(socket, message, message_length) );

    Socket_Close(socket);
}

TEST(Socket, it_can_send_data_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char message[] = "Hello";
    unsigned int message_length = sizeof(message);
    int number_of_bytes_sent = 1;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketSend(file_descriptor, message, message_length, number_of_bytes_sent);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( number_of_bytes_sent, Socket_Send(socket, message, message_length) );

    Socket_Close(socket);
}

// Receive
TEST(Socket, it_can_fail_to_receive_from_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char receive_buffer[10] = {0};
    unsigned int receive_buffer_length = 9;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketReceive(file_descriptor, receive_buffer, receive_buffer_length, UNIX_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Receive(socket, receive_buffer, receive_buffer_length) );

    Socket_Close(socket);
}

TEST(Socket, it_will_not_receive_with_a_null_buffer)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    unsigned int receive_buffer_length = 9;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Receive(socket, NULL, receive_buffer_length) );

    Socket_Close(socket);
}

TEST(Socket, it_will_not_receive_if_buffer_length_is_zero)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char receive_buffer[10] = {0};
    unsigned int receive_buffer_length = 0;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Receive(socket, receive_buffer, receive_buffer_length) );

    Socket_Close(socket);
}

TEST(Socket, it_can_receive_from_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char receive_buffer[10] = {0};
    unsigned int receive_buffer_length = 9;
    int number_of_bytes_read = 1;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketReceive(file_descriptor, receive_buffer, receive_buffer_length, number_of_bytes_read);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( number_of_bytes_read, Socket_Receive(socket, receive_buffer, receive_buffer_length) );

    Socket_Close(socket);
}

// Listen
TEST(Socket, it_can_fail_to_listen_on_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 8888;
    int backlog = 3;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);    //TODO any ip address
    expectSocketListen(file_descriptor, backlog, UNIX_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Listen(socket, backlog) );

    Socket_Close(socket);
}

TEST(Socket, it_can_to_listen_on_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 8888;
    int backlog = 3;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);    //TODO any ip address
    expectSocketListen(file_descriptor, backlog, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Listen(socket, backlog) );

    Socket_Close(socket);
}

// Accept
TEST(Socket, it_can_fail_to_accept_a_socket_connection)
{
    const char * ip_address = "192.168.2.1";
    int port = 8888;
    int backlog = 3;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);    //TODO any ip address
    expectSocketListen(file_descriptor, backlog, UNIX_SOCKET_SUCCESS);
    expectSocketAccept(file_descriptor, UNIX_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);
    Socket_Listen(socket, backlog);

    POINTERS_EQUAL( NULL, Socket_Accept(socket) );

    Socket_Close(socket);
}

TEST(Socket, it_can_accept_a_socket_connection)
{
    Socket new_socket;
    int new_file_descriptor = 44;
    const char * ip_address = "192.168.2.1";
    int port = 8888;
    int backlog = 3;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, UNIX_SOCKET_SUCCESS);    //TODO any ip address
    expectSocketListen(file_descriptor, backlog, UNIX_SOCKET_SUCCESS);
    expectSocketAccept(file_descriptor, new_file_descriptor);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);
    Socket_Listen(socket, backlog);

    new_socket = Socket_Accept(socket);
    LONGS_EQUAL( new_file_descriptor, Socket_GetFileDescriptor(new_socket) );

    // Should we verify the client address and client address length here?

    Socket_Close(socket);
    Socket_Destroy(&new_socket);
}
