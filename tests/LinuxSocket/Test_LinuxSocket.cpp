extern "C"
{
#include "LinuxSocket.h"
}

#include "Test_LinuxSocket.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(LinuxSocket)
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

/* Test List:
 *  Create()
 *
 *  Destroy()
 */

TEST(LinuxSocket, it_can_fail)
{
    FAIL("flunk");
}
