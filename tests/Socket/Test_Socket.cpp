extern "C"
{
#include "Socket.h"
#include "SocketSystemLayer.h"
}

#include "Test_Socket.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(Socket)
{
    Socket socket;
    int socket_descriptor;

    void setup()
    {
        mock().strictOrder();
        socket = Socket_Create();
        socket_descriptor = 42;
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        mock().checkExpectations();
        mock().clear();
    }

    void expectSocketOpen(int socket_descriptor_or_error_code)
    {
        mock().expectOneCall("SocketSystemLayer_Open")
            .andReturnValue(socket_descriptor_or_error_code);
    }

    void expectSocketClose(int socket_descriptor, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Close")
            .withParameter("descriptor", socket_descriptor)
            .andReturnValue(return_code);
    }

    void expectSocketBind(int socket_descriptor, const char *ip_address, int port, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Bind")
            .withParameter("descriptor", socket_descriptor)
            .withParameter("ip_address", ip_address)
            .withParameter("port", port)
            .andReturnValue(return_code);
    }

    void expectSocketListen(int socket_descriptor, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Listen")
            .withParameter("descriptor", socket_descriptor)
            .withParameter("backlog", 0)
            .andReturnValue(return_code);
    }
};

#define CHECK_SOCKET_RESET(socket) \
    LONGS_EQUAL( SOCKET_INVALID_DESCRIPTOR, Socket_GetDescriptor((socket)) ); \
    CHECK_SOCKET_ADDRESS_AND_PORT((socket), SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);

#define CHECK_SOCKET_ADDRESS_AND_PORT(socket, address, port) \
    LONGS_EQUAL( (address), Socket_GetIpAddress((socket)) ); \
    LONGS_EQUAL( (port), Socket_GetPort((socket)) );

/*
 * Test List:
 *  Create:
 *
 *  Destroy:
 *      Do we need to check *self == 0?
 *
 *  Open:
 *      Do all open tests need to close the socket as well?
 *
 *  Close:
 *
 *  Connect (client only):
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Bind (server only):
 *      Bind to any address.
 *
 *  Listen (server only):
 *
 *  Accept (server only):
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Send:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Receive:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Get handle [file descriptor]:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Get IP address:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Get port:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Set option:
 *      Can fail.
 *      Will not crash with null pointer.
 */

// Create and destroy
TEST(Socket, it_can_create_and_destroy_a_socket)
{
    CHECK_SOCKET_RESET(socket);
}

TEST(Socket, it_can_destroy_a_socket_twice)
{
    Socket_Destroy(&socket);
}

// Null pointers
TEST(Socket, it_can_handle_null_pointers)
{
    Socket_Destroy(NULL);
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_GetDescriptor(NULL) );
    Socket_Close(NULL);
    POINTERS_EQUAL( SOCKET_INVALID_IP_ADDRESS, Socket_GetIpAddress(NULL) );
    LONGS_EQUAL( SOCKET_INVALID_PORT, Socket_GetPort(NULL) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Listen(NULL) );
}

TEST(Socket, bind_can_accept_null_pointers)
{
    const char * ip_address = "192.168.0.1";
    int port = 12345;

    Socket_Bind(NULL, ip_address, port);
}

// Open
TEST(Socket, it_can_open_a_socket)
{
    expectSocketOpen(socket_descriptor);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open(socket) );

    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);
}

TEST(Socket, it_can_fail_to_open_a_socket)
{
    expectSocketOpen(SOCKET_SYSTEM_LAYER_FAIL);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Open(socket) );

    LONGS_EQUAL( SOCKET_INVALID_DESCRIPTOR, Socket_GetDescriptor(socket) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);
}

TEST(Socket, it_can_open_several_sockets)
{
    Socket socket2 = Socket_Create();
    int socket_descriptor2 = 66;
    expectSocketOpen(socket_descriptor);
    expectSocketOpen(socket_descriptor2);

    Socket_Open(socket);
    Socket_Open(socket2);

    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);
    LONGS_EQUAL( socket_descriptor2, Socket_GetDescriptor(socket2) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket2, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);

    Socket_Destroy(&socket2);
}

TEST(Socket, it_can_not_open_the_same_socket_twice)
{
    expectSocketOpen(socket_descriptor);
    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_ALREADY_OPEN, Socket_Open(socket) );
    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
}

// Close
TEST(Socket, it_can_close_a_socket)
{
    expectSocketOpen(socket_descriptor);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket);

    Socket_Close(socket);

    CHECK_SOCKET_RESET(socket);
}

TEST(Socket, it_can_close_several_sockets)
{
    Socket socket2 = Socket_Create();
    int socket_descriptor2 = 66;
    expectSocketOpen(socket_descriptor);
    expectSocketOpen(socket_descriptor2);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_FAIL);
    expectSocketClose(socket_descriptor2, SOCKET_SYSTEM_LAYER_FAIL);
    Socket_Open(socket);
    Socket_Open(socket2);

    Socket_Close(socket);
    Socket_Close(socket2);

    CHECK_SOCKET_RESET(socket);
    CHECK_SOCKET_RESET(socket2);

    Socket_Destroy(&socket2);
}

TEST(Socket, it_will_not_close_a_socket_twice)
{
    expectSocketOpen(socket_descriptor);
    // Note that there is only one call to the close.
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket);
    Socket_Close(socket);

    Socket_Close(socket);

    CHECK_SOCKET_RESET(socket);
}

// Bind
TEST(Socket, it_can_bind_to_a_specific_ip_address_and_port)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor);
    expectSocketBind(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Bind(socket, ip_address, port) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, ip_address, port);
}

TEST(Socket, closing_a_bound_socket_resets_ip_address_and_port)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor);
    expectSocketBind(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);

    Socket_Close(socket);
    CHECK_SOCKET_RESET(socket);
}

// TEST(Socket, it_can_bind_to_any_ip_address_and_port)
// {
//     // TODO
// }

TEST(Socket, it_can_not_bind_to_an_address_and_port_twice)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor);
    expectSocketBind(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketBind(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_ADDRESS_IN_USE);
    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_ADDRESS_IN_USE, Socket_Bind(socket, ip_address, port) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, ip_address, port);
}

TEST(Socket, it_can_fail_to_bind)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor);
    expectSocketBind(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_FAIL);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Bind(socket, ip_address, port) );

    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);

    Socket_Close(socket);
}

// Listen
TEST(Socket, it_can_listen)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor);
    expectSocketBind(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Listen(socket) );
}

TEST(Socket, it_can_fail_to_listen)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor);
    expectSocketBind(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, SOCKET_SYSTEM_LAYER_FAIL);
    Socket_Open(socket);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Listen(socket) );
}
