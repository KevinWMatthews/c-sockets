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

    void setup()
    {
        mock().strictOrder();
        socket = Socket_Create();
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        mock().checkExpectations();
        mock().clear();
    }
};

/*
 * Test List:
 *  Create:
 *
 *  Destroy:
 *      Null pointers.
 *      Double destroy.
 *      Do we need to check *self == 0?
 *
 *  Open:
 *
 *  Close:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Connect (client only):
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Bind (server only):
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Listen (server only):
 *      Can fail.
 *      Will not crash with null pointer.
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

TEST(Socket, it_can_create_and_destroy_a_socket)
{
}

TEST(Socket, it_can_destroy_a_socket_twice)
{
    Socket_Destroy(&socket);
}

TEST(Socket, it_can_handle_null_pointers)
{
    Socket_Destroy(NULL);
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_GetDescriptor(NULL) );
}

TEST(Socket, it_can_fail_to_open_a_socket)
{
    mock().expectOneCall("SocketSystemLayer_Open")
        .andReturnValue(SOCKET_SYSTEM_LAYER_FAIL);
    LONGS_EQUAL( SOCKET_FAIL, Socket_Open(socket) );
    LONGS_EQUAL( SOCKET_INVALID_DESCRIPTOR, Socket_GetDescriptor(socket) );
}

TEST(Socket, it_can_open_a_socket)
{
    int socket_descriptor = 42;

    mock().expectOneCall("SocketSystemLayer_Open")
        .andReturnValue(socket_descriptor);
    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open(socket) );
    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
}
