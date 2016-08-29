#include <stdio.h>
#include "SocketServer.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

void close_and_destroy_socket(Socket *socket)
{
    printf("Closing socket...\n");
    Socket_Close(*socket);
    Socket_Destroy(socket);
}

static void * socket_handler_thread(void * client_socket)
{
    Socket socket = *(Socket *)client_socket;
    const char * confirm_connection = "The server has accepted the connection.\n";
    char buffer[2000] = {0};

    if ( Socket_Send( socket, confirm_connection, strlen(confirm_connection) ) < 0 )
    {
        printf("Send failed!\n");
        close_and_destroy_socket(&socket);
        exit(EXIT_FAILURE);
        return NULL;
    }

    while (1)
    {
        memset( buffer, 0, sizeof(buffer) );
        Socket_Receive( socket, buffer, sizeof(buffer) );
        printf("Received data from client. Sending response to client...\n");
        if ( Socket_Send(socket, buffer, sizeof(buffer) ) < 0 )
        {
            printf("Send failed!\n");
            close_and_destroy_socket(&socket);
            exit(EXIT_FAILURE);
            return NULL;
        }
        printf("Message sent\n");
    }

    exit(EXIT_FAILURE);
    return NULL;
}

int main(void)
{
    Socket socket = Socket_Create();
    Socket client_socket = {0};

    printf("Opening socket...\n");
    if ( Socket_Open(socket) < 0 )
    {
        printf("Open failed!\n");
        Socket_Destroy(&socket);
        return 1;
    }

    if ( Socket_SetOption(socket, SOCKET_IMMEDIATELY_REUSE_SOCKET) < 0 )
    {
        printf("Setting socket option failed!\n");
        Socket_Destroy(&socket);
        return 1;
    }

    printf("Binding socket...\n");
    if ( Socket_Bind(socket, 0, 8888) < 0 )
    {
        printf("Bind failed!\n");
        close_and_destroy_socket(&socket);
        return 1;
    }

    printf("Listening on socket...\n");
    if ( Socket_Listen(socket, 0) < 0 ) //TODO experiment with the backlog?
    {
        printf("Listen failed!\n");
        close_and_destroy_socket(&socket);
        return 1;
    }

    printf("Waiting to accept a connection...\n");
    printf("To connect to this socket, open a new terminal window and type:\ntelnet localhost 8888\n");
    do
    {
        client_socket = Socket_Accept(socket);
        if (!client_socket)
        {
            printf("Accept failed!\n");
            close_and_destroy_socket(&socket);
            return 1;
        }
        else
        {
            pthread_t thread_handle;
            if ( pthread_create( &thread_handle, NULL, socket_handler_thread, (void *)&client_socket) < 0)
            {
                printf("Failed to create thread\n");
                // TODO How to close all of the sockets?
                close_and_destroy_socket(&socket);
                close_and_destroy_socket(&client_socket);
                return 1;
            }

        }
        printf("Connection accepted.\n");
    } while (1);

    close_and_destroy_socket(&socket);
    close_and_destroy_socket(&client_socket);
    return 0;
}
