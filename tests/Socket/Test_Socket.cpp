extern "C"
{
#include "Socket.h"
#include "DummySocket.h"
}

#include "Test_Socket.h"
#include "SocketHelpers.h"
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
        socket = DummySocket_Create();
        socket2 = DummySocket_Create();
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        Socket_Destroy(&socket2);
        mock().checkExpectations();
        mock().clear();
    }
};

/* Test List:
 *  Create:
 *      Null interface
 *
 *  GetIpAddress:
 *
 *  GetPort:
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
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Send(NULL, "msg", 3) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Receive(NULL, buffer, 10) );
    Socket_Close(NULL);
}

// Open
TEST(Socket, it_can_fail_to_open_a_socket)
{
    expectSocketOpen(DUMMY_SOCKET_FAIL);
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

// Connect
TEST(Socket, it_can_fail_to_connect_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_FAIL);
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
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Connect(socket, ip_address, port) );
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
    expectSocketBind(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
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
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
    expectSocketSend(file_descriptor, message, message_length, DUMMY_SOCKET_FAIL);
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
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
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
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
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
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
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
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
    expectSocketReceive(file_descriptor, receive_buffer, receive_buffer_length, DUMMY_SOCKET_FAIL);
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
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
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
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
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
    expectSocketConnect(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
    expectSocketReceive(file_descriptor, receive_buffer, receive_buffer_length, number_of_bytes_read);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( number_of_bytes_read, Socket_Receive(socket, receive_buffer, receive_buffer_length) );

    Socket_Close(socket);
}
