extern "C"
{
#include "LinuxSocket.h"
}

#include "Test_LinuxSocket.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

/* Test List:
 *  Create()
 *      Create the interface
 *
 *  Destroy()
 *      Double destroy
 *      Figure out how to set the pointer to NULL.
 *
 *  SetOption:
 *      Linux-specific stuff.
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

TEST(LinuxSocket, it_can_create_and_destroy_a_socket)
{
    //TODO
    // POINTERS_EQUAL(NULL, socket);
}

TEST(LinuxSocket, it_can_handle_null_pointers)
{
    LinuxSocket_Destroy(&socket);
    // LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_SetOption(NULL, SOCKET_IMMEDIATELY_REUSE_SOCKET) );
}

// Set options
// IGNORE_TEST(Socket, it_can_fail_to_set_a_socket_option)
// {
//     expectSocketOpen(file_descriptor);
//     expectSocketSetOption(file_descriptor, UNIX_SOCKET_IMMEDIATELY_REUSE_SOCKET, DUMMY_SOCKET_FAIL);
//     expectSocketClose(file_descriptor);

//     Socket_Open(socket);
//     LONGS_EQUAL( SOCKET_FAIL, Socket_SetOption(socket, SOCKET_IMMEDIATELY_REUSE_SOCKET) );
//     Socket_Close(socket);
// }

// IGNORE_TEST(Socket, it_can_set_socket_option_immediate_reuse)
// {
//     expectSocketOpen(file_descriptor);
//     expectSocketSetOption(file_descriptor, UNIX_SOCKET_IMMEDIATELY_REUSE_SOCKET, DUMMY_SOCKET_SUCCESS);
//     expectSocketClose(file_descriptor);

//     Socket_Open(socket);
//     LONGS_EQUAL( SOCKET_SUCCESS, Socket_SetOption(socket, SOCKET_IMMEDIATELY_REUSE_SOCKET) );
//     Socket_Close(socket);
// }
