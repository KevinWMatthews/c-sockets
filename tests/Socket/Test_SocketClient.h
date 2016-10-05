#ifndef TEST_SOCKET_CLIENT_INCLUDED
#define TEST_SOCKET_CLIENT_INCLUDED

class Test_SocketClient
{
  public:
    explicit Test_SocketClient();
    virtual ~Test_SocketClient();

  private:
    Test_SocketClient(const Test_SocketClient&);
    Test_SocketClient& operator=(const Test_SocketClient&);
};

#endif
