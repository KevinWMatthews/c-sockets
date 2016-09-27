#include <stdio.h>
#include "Socket.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

static void close_and_destroy_socket(Socket *socket)
{
    printf("Closing socket...\n");
    Socket_Close(*socket);
    Socket_Destroy(socket);
}

static void * socket_handler_thread(void * client_socket)
{
    Socket socket = *(Socket *)client_socket;
    char * confirm_connection = "The server has accepted the connection.\n";
    char buffer[2000] = {0};

    if ( Socket_Send( socket, confirm_connection, strlen(confirm_connection) ) < 0 )
    {
        perror("Send failed");
        close_and_destroy_socket(&socket);
        exit(EXIT_FAILURE);
        return NULL;
    }

    while (1)
    {
        memset( buffer, 0, sizeof(buffer) );
        Socket_Receive( socket, buffer, sizeof(buffer) );
        printf("Received data from client:\n");
        printf("%s\n", buffer);
        printf("Sending response to client...\n");
        if ( Socket_Send(socket, buffer, sizeof(buffer) ) < 0 )
        {
            perror("Send failed");
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
    Socket server_socket = 0;
    Socket client_socket = 0;

    printf("Server starting up...\n");

    printf("Creating server socket...\n");
    server_socket = Socket_Create();
    if (server_socket == 0)
    {
        printf("Could not create server Socket!\n");
        return 1;
    }

    printf("Opening socket...\n");
    if ( Socket_Open(server_socket) < 0 )
    {
        perror("Open failed");
        Socket_Destroy(&server_socket);
        return 1;
    }

    printf("Binding socket...\n");
    if ( Socket_Bind(server_socket, "127.0.0.1", 8888) < 0 )
    {
        perror("Bind failed");
        close_and_destroy_socket(&server_socket);
        return 1;
    }

    printf("Listening on socket...\n");
    if ( Socket_Listen(server_socket) < 0 ) //TODO experiment with the backlog?
    {
        perror("Listen failed");
        close_and_destroy_socket(&server_socket);
        return 1;
    }

    printf("Waiting to accept a connection...\n");
    printf("To connect to this socket, open a new terminal window and type:\ntelnet localhost 8888\n");
    do
    {
        client_socket = Socket_Accept(server_socket);
        if (!client_socket)
        {
            perror("Accept failed");
            close_and_destroy_socket(&server_socket);
            return 1;
        }
        else
        {
            pthread_t thread_handle;
            if ( pthread_create( &thread_handle, NULL, socket_handler_thread, (void *)&client_socket) < 0)
            {
                printf("Failed to create thread\n");
                // TODO How to close all of the sockets?
                close_and_destroy_socket(&server_socket);
                close_and_destroy_socket(&client_socket);
                return 1;
            }
        }
        printf("Connection accepted.\n");
    } while (1);

    printf("Closing and destroying sockets...\n");
    close_and_destroy_socket(&server_socket);
    close_and_destroy_socket(&client_socket);

    printf("Server stopping!\n");
    return 0;
}
