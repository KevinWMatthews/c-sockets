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

TEST(UnixSocket, it_can_fail_to_open_a_socket)
{
    mock().expectOneCall("UnixSocket_Open")
        .andReturnValue(UNIX_SOCKET_FAIL);
    LONGS_EQUAL( SOCKET_FAIL, Socket_Open() );
}

TEST(UnixSocket, it_can_open_a_socket)
{
    int file_descriptor = 42;   // Or whatever.
    mock().expectOneCall("UnixSocket_Open")
        .andReturnValue(file_descriptor);

    LONGS_EQUAL( file_descriptor, Socket_Open() );
}

TEST(UnixSocket, it_can_close_a_socket)
{
    int file_descriptor = 42;
    mock().expectOneCall("UnixSocket_Close")
        .withParameter("file_descriptor", file_descriptor);

    Socket_Close(file_descriptor);
}
