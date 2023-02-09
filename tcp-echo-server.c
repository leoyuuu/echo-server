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

    struct sockaddr_in server_addr, client_addr;
    int socketFd, clientFd;
    unsigned int client_addr_len = sizeof(client_addr);

    memset(&client_addr, 0, sizeof(client_addr));
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketFd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(socketFd, 5);
    while (1)
    {
        clientFd = accept(socketFd, (struct sockaddr *)&client_addr, &client_addr_len);
        pthread_t t;
        struct Client *c = (struct Client *)malloc(sizeof(struct Client));
        memcpy(&c->client_addr, &client_addr, sizeof(client_addr));
        c->client_fd = clientFd;
        pthread_create(&t, NULL, handle_client, c);
    }
    close(socketFd);

    return 0;
}
