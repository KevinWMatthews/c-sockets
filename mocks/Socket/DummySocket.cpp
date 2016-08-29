#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "DummySocket.h"
}

static SocketInterfaceStruct interface_struct;
static SocketInterface interface = &interface_struct;

int DummySocket_Open(void)
{
    mock().actualCall("DummySocket_Open");
    return mock().intReturnValue();
}

void DummySocket_Close(int file_descriptor)
{
    mock().actualCall("DummySocket_Close")
        .withParameter("file_descriptor", file_descriptor);
}

int DummySocket_Send(int file_descriptor, const char * message, unsigned int message_length)
{
    mock().actualCall("DummySocket_Send")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("message", message)
        .withParameter("message_length", message_length);
    return mock().intReturnValue();
}

int DummySocket_Receive(int file_descriptor, char * buffer, unsigned int buffer_length)
{
    mock().actualCall("DummySocket_Receive")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("buffer", buffer)
        .withParameter("buffer_length", buffer_length);
    return mock().intReturnValue();
}

int DummySocket_Connect(int file_descriptor, const char * ip_address, int port)
{
    mock().actualCall("DummySocket_Connect")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port);
    return mock().intReturnValue();
}

int DummySocket_Bind(int file_descriptor, const char * ip_address, int port)
{
    mock().actualCall("DummySocket_Bind")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("ip_address", ip_address)
        .withParameter("port", port);
    return mock().intReturnValue();
}

int DummySocket_Accept(int file_descriptor)
{
    mock().actualCall("DummySocket_Accept")
        .withParameter("file_descriptor", file_descriptor);
    return mock().intReturnValue();
}

int DummySocket_Listen(int file_descriptor, int backlog)
{
    mock().actualCall("DummySocket_Listen")
        .withParameter("file_descriptor", file_descriptor)
        .withParameter("backlog", backlog);
    return mock().intReturnValue();
}

Socket DummySocket_Create(void)
{
    interface->open = DummySocket_Open;
    interface->close = DummySocket_Close;
    interface->send = DummySocket_Send;
    interface->receive = DummySocket_Receive;
    interface->connect = DummySocket_Connect;
    interface->bind = DummySocket_Bind;
    interface->accept = DummySocket_Accept;
    interface->listen = DummySocket_Listen;
    return Socket_Create(interface);
}
