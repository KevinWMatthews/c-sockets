extern "C"
{
#include "Socket.h"
#include "UnixSocket.h"
}

#include "Test_UnixSocket.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(UnixSocket)
{
    void setup()
    {
        mock().strictOrder();
    }

    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(UnixSocket, it_can_fail_to_create_a_socket)
{
    mock().expectOneCall("UnixSocket_Create")
        .andReturnValue(UNIX_SOCKET_FAIL);
    LONGS_EQUAL( SOCKET_FAIL, Socket_Create() );
}
