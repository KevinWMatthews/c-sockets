extern "C"
{
#include "LinuxSocket.h"
#include "LinuxSocketOs.h"
}

#include "Test_LinuxSocket.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(LinuxSocket)
{
    Socket socket;

    void setup()
    {
        mock().strictOrder();
        socket = LinuxSocket_Create();
    }

    void teardown()
    {
        LinuxSocket_Destroy(&socket);
        mock().checkExpectations();
        mock().clear();
    }
};

/* Test List:
 *  Create()
 *
 *  Destroy()
 *
 *  Open()
 *  Close()
 *  Bind()
 *  Connect()
 *  Send()
 *  Receive()
 *  Listen()
 *  Accept()
 *  GetPort()
 *  GetIpAddress()
 *  GetFileDescriptor()
 */

TEST(LinuxSocket, it_can_create_and_destroy_a_socket)
{
    LinuxSocket_Destroy(&socket);
}

TEST(LinuxSocket, it_fails_to_open_if_socket_is_null)
{
    LONGS_EQUAL( LINUX_SOCKET_FAIL, LinuxSocket_Open(NULL, LINUX_SOCKET_IPV4) );
}
