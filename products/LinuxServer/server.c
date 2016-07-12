#include <stdio.h>
#include "Socket.h"
#include <string.h>

void close_and_destroy_socket(Socket *socket)
{ printf("Closing socket...\n");
    Socket_Close(*socket);
    Socket_Destroy(socket);
}

int main(int argc, char * argv[])
{
    Socket socket = Socket_Create();
    Socket client_socket = {0};
    const char * confirm_connection = "The server has accepted the connection.\n";
    size_t confirm_connection_len = strlen(confirm_connection);

    printf("Opening socket...\n");
    if ( Socket_Open(socket) < 0 )
    {
        printf("Open failed!\n");
        Socket_Destroy(&socket);
        return 0;
    }

    printf("Binding socket...\n");
    if ( Socket_Bind(socket, 0, 8888) < 0 )
    {
        printf("Bind failed!\n");
        close_and_destroy_socket(&socket);
        return 0;
    }

    printf("Listening on socket...\n");
    if ( Socket_Listen(socket, 0) < 0 ) //TODO experiment with the backlog?
    {
        printf("Listen failed!\n");
        close_and_destroy_socket(&socket);
        return 0;
    }

    printf("Waiting to accept a connection...\n");
    printf("To connect to this socket, open a new terminal window and type:\ntelnet localhost 8888\n");
    client_socket = Socket_Accept(socket);
    if (!client_socket)
    {
        printf("Accept failed!\n");
        close_and_destroy_socket(&socket);
        return 0;
    }
    printf("Connection accepted.\n");

    printf("Sending response to client...\n");
    if ( Socket_Send(client_socket, confirm_connection, confirm_connection_len) < 0 )
    {
        printf("Send failed!\n");
        close_and_destroy_socket(&socket);
        close_and_destroy_socket(&client_socket);
        return 0;
    }
    printf("Message sent\n");

    close_and_destroy_socket(&socket);
    close_and_destroy_socket(&client_socket);
    return 0;
}
