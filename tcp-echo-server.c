#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
struct Client
{
    int client_fd;
    struct sockaddr_in client_addr;
};

void *handle_client(void *arg)
{
    struct Client *c = (struct Client *)arg;
    int receivedLength;
    char buffer[BUFFER_SIZE];
    int clientFd = c->client_fd;
    while (1)
    {
        receivedLength = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);
        if (receivedLength <= 0)
        {
            break;
        }
        buffer[receivedLength] = '\0';
        printf("%s:%u says: %s\n", inet_ntoa(c->client_addr.sin_addr), ntohs(c->client_addr.sin_port), buffer);
        send(clientFd, buffer, receivedLength, 0);
    }
    printf("%s:%u end\n", inet_ntoa(c->client_addr.sin_addr), ntohs(c->client_addr.sin_port));
    close(clientFd);
    free(arg);
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage:%s <port>\n", argv[0]);
        return 0;
    }

    struct sockaddr_in myAddr, clientAddr;
    int socketFd, clientFd;
    unsigned int clientAddrLen = sizeof(clientAddr);

    memset(&clientAddr, 0, sizeof(clientAddr));
    memset(&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(atoi(argv[1]));
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketFd, (struct sockaddr *)&myAddr, sizeof(myAddr));
    listen(socketFd, 5);
    while (1)
    {
        clientFd = accept(socketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        pthread_t t;
        struct Client *c = (struct Client *)malloc(sizeof(struct Client));
        memcpy(&c->client_addr, &clientAddr, sizeof(clientAddr));
        c->client_fd = clientFd;
        pthread_create(&t, NULL, handle_client, c);
    }
    close(socketFd);

    return 0;
}
