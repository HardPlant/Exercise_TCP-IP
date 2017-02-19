#include <sys/socket.h> // socket functions
#include <arpa/inet.h> // for socket support
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024
int main(int argc, char **argv)
{
    struct sockaddr_in serveraddr;
    int server_sockfd;
    int client_len;
    char buf[MAXLINE];

    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    if(-1 == server_sockfd)
    {
        perror("[ERROR] :");
        return -1;
    }

    char addr[] = "127.0.01";
    int port = 3500;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(addr);
    serveraddr.sin_port = htons(port);

    client_len = sizeof(serveraddr);

    if(-1 == connect(server_sockfd, (struct sockaddr*) &serveraddr,
                client_len))
    {
        perror("[ERROR] CONNECT:");
        return -1;
    }

    memset(buf,0x00,MAXLINE);
    read(0,buf,MAXLINE);
    if(write(server_sockfd, buf, MAXLINE) <= 0)
    {
        perror("[ERROR] WRITE:");
        return -1;
    }
    memset(buf,0x00,MAXLINE);
    if(read(server_sockfd, buf, MAXLINE <= 0))
    {
        perror("[ERROR] READ:");
        return -1;
    }
    close(server_sockfd);
    printf("read : %s",buf);
    return 0;
}
