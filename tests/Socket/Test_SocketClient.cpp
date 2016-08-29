extern "C"
{
#include "Socket.h"
#include "SocketClient.h"
#include "DummySocket.h"
}

#include "Test_Socket.h"
#include "SocketHelpers.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

/* Test List:
 *  Connect:
 *      Fail with null Socket pointer.
 *      Low-level library can fail.
 *      Error if passed invalid IP address.
 *      Error if passed invalid port.
 *      Success.
 */

TEST_GROUP(SocketClient)
{
    int file_descriptor;
    Socket socket;

    void setup()
    {
        mock().strictOrder();
        file_descriptor = 42;
        socket = DummySocket_Create();
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(SocketClient, it_can_handle_null_pointers)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Connect(NULL, "0.0.0.0", 0) );
}
