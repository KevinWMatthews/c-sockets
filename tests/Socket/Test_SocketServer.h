#ifndef TEST_SOCKET_SERVER_INCLUDED
#define TEST_SOCKET_SERVER_INCLUDED

class Test_SocketServer
{
  public:
    explicit Test_SocketServer();
    virtual ~Test_SocketServer();

  private:
    Test_SocketServer(const Test_SocketServer&);
    Test_SocketServer& operator=(const Test_SocketServer&);
};

#endif
