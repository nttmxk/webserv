

#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>


#include <unistd.h>
#define PORT 8080



void HandleTCPClient(int clntSocket);
int server_fd, new_socket; long valread;

int main(int argc, char const *argv[])
{

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    // setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {

        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

 		// HandleTCPClient(new_socket);
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
      //  write(new_socket , hello , strlen(hello));
        if (send(new_socket, "hello", strlen(hello), 0) == -1)
            printf("send() failed");
        printf("------------------Hello message sent-------------------\n");

        close(new_socket);
    }

	printf("hihi\n");
    return 0;
}



void HandleTCPClient(int clntSocket)
{
    char echoBuffer[32];
    int recvMsgSize;

    if ((recvMsgSize = recv(clntSocket, echoBuffer, 32, 0)) < 0)
	{
        perror("recv() failed");
        exit(EXIT_FAILURE);
    }
    

    fprintf(stderr, "echoBuffer = %s \n", echoBuffer);
    fprintf(stderr, "recvMsgSize = %d \n", recvMsgSize);

    while (recvMsgSize > 0)
    {
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
		{
            perror("send() failed");
            exit(EXIT_FAILURE);
        }
    

        if ((recvMsgSize = recv(clntSocket, echoBuffer, 32, 0)) < 0)
		{
            perror("recv() failed");
            exit(EXIT_FAILURE);
        }
    }

    close(clntSocket);
}
