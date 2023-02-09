#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define BUF_SIZE 1500

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage:%s <port>\n", argv[0]);
        return 0;
    }

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int res = bind(socket_fd, (struct sockaddr*)(&server_addr), sizeof(server_addr));
    if(res != 0) {
        printf("error launch udp server %d\n", res);
        return -1;
    }
    char buf[BUF_SIZE];
    while(1) {
        u_int32_t len = sizeof(client_addr);
        int recv_len = recvfrom(socket_fd, buf, BUF_SIZE-2, 0, (struct sockaddr*)&client_addr, &len);
        buf[recv_len] = 0;
        printf("%s:%d says: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
        sendto(socket_fd, buf, recv_len, 0, (struct sockaddr*)(&client_addr), sizeof(client_addr));
    }

    return 0;
}
