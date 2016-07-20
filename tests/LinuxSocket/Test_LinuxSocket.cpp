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

TEST(LinuxSocket, it_can_fail_to_open_a_socket)
{
    mock().expectOneCall("LinuxSocketOs_Open")
        .withParameter("domain", LINUX_SOCKET_OS_AF_INET)
        .andReturnValue(LINUX_SOCKET_OS_FAIL);
    LONGS_EQUAL( LINUX_SOCKET_FAIL, LinuxSocket_Open(socket, LINUX_SOCKET_IPV4) );
    // LONGS_EQUAL( -1, Socket_GetFileDescriptor(socket) );
}

TEST(LinuxSocket, it_can_open_an_ipv4_socket)
{
    mock().expectOneCall("LinuxSocketOs_Open")
        .withParameter("domain", LINUX_SOCKET_OS_AF_INET)
        .andReturnValue(LINUX_SOCKET_OS_SUCCESS);
    LONGS_EQUAL( LINUX_SOCKET_SUCCESS, LinuxSocket_Open(socket, LINUX_SOCKET_IPV4) );
    // LONGS_EQUAL( -1, Socket_GetFileDescriptor(socket) );
}
