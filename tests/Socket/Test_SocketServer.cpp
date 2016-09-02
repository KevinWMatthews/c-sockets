extern "C"
{
#include "Socket.h"
#include "DummySocket.h"
}

#include "Test_Socket.h"
#include "SocketHelpers.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

/* Test List:
 *  Bind:
        Fail with null Socket pointer.
        Low-level library can fail.
 *      Error if passed invalid IP address.
 *      Error if passed invalid port.
        Success

 * Listen:
        Fail with null Socket pointer.
        Low-level library can fail.
        Success.
 *      Backlog parameter.
 *
 *  Accept:
        Fail with null Socket pointer.
        Low-level library can fail.
 *      Null address - is valid, I think.
 *      Null size pointer - is valid if address is null.
 *      Should we verify the contents of the client address and client address length?
 *      Accept on any IP address.
        Success
 *
 */

TEST_GROUP(SocketServer)
{
    int file_descriptor;
    Socket socket;

    void setup()
    {
        mock().strictOrder();
        file_descriptor = 66;
        socket = DummySocket_Create();
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(SocketServer, it_can_handle_null_pointers)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Bind(NULL, "0.0.0.0", 0) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Listen(NULL, 10) );
    POINTERS_EQUAL( NULL, Socket_Accept(NULL) );
}

// Bind
TEST(SocketServer, it_can_fail_to_bind_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, DUMMY_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Bind(socket, ip_address, port) );
    POINTERS_EQUAL( NULL, Socket_GetIpAddress(socket) );
    LONGS_EQUAL( SOCKET_INVALID_PORT, Socket_GetPort(socket) );

    Socket_Close(socket);
}

TEST(SocketServer, it_will_bind_to_any_ip_address_with_a_null_ip_address_pointer)
{
    const char * ip_address = NULL;
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Bind(socket, ip_address, port) );

    Socket_Close(socket);
}

TEST(SocketServer, it_can_to_bind_to_a_specific_ip_and_port)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Bind(socket, ip_address, port) );
    STRCMP_EQUAL( ip_address, Socket_GetIpAddress(socket) );
    LONGS_EQUAL( port, Socket_GetPort(socket) );

    Socket_Close(socket);
}

// Listen
TEST(SocketServer, it_can_fail_to_listen_on_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 8888;
    int backlog = 3;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);    //TODO any ip address
    expectSocketListen(file_descriptor, backlog, DUMMY_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Listen(socket, backlog) );

    Socket_Close(socket);
}

TEST(SocketServer, it_can_to_listen_on_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 8888;
    int backlog = 3;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);    //TODO any ip address
    expectSocketListen(file_descriptor, backlog, DUMMY_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Listen(socket, backlog) );

    Socket_Close(socket);
}

// Accept
TEST(SocketServer, it_can_fail_to_accept_a_socket_connection)
{
    const char * ip_address = "192.168.2.1";
    int port = 8888;
    int backlog = 3;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);    //TODO any ip address
    expectSocketListen(file_descriptor, backlog, DUMMY_SOCKET_SUCCESS);
    expectSocketAccept(file_descriptor, DUMMY_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);
    Socket_Listen(socket, backlog);

    POINTERS_EQUAL( NULL, Socket_Accept(socket) );

    Socket_Close(socket);
}

TEST(SocketServer, it_can_accept_a_socket_connection)
{
    Socket new_socket;
    int new_file_descriptor = 44;
    const char * ip_address = "192.168.2.1";
    int port = 8888;
    int backlog = 3;

    expectSocketOpen(file_descriptor);
    expectSocketBind(file_descriptor, ip_address, port, DUMMY_SOCKET_SUCCESS);    //TODO any ip address
    expectSocketListen(file_descriptor, backlog, DUMMY_SOCKET_SUCCESS);
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
