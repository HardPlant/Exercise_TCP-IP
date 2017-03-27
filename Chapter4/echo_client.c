#include <sys/socket.h> // socket functions
#include <arpa/inet.h> // for socket support
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024
int make_server_sockfd()
{
    return socket(AF_INET,SOCK_STREAM,0);
}
int bufferToServer(int server_sockfd, char* buf)
{
    read(0,buf,MAXLINE);
    if(write(server_sockfd, buf, MAXLINE) <= 0)
    {
        perror("[ERROR] WRITE:");
        return -1;
    }
    return 0;
}
int serverToBuffer(int server_sockfd, char* buf)
{
    read(server_sockfd,buf,MAXLINE);
    if(read(server_sockfd, buf, MAXLINE <= 0))
    {
        perror("[ERROR] READ:");
        return -1;
    }
    return 0;
}
int isargentered(char* argv)
{
    if(strcmp(argv,"\0") == 0) return 0;
    else return 1;
    
}
int main(int argc, char **argv)
{
    struct sockaddr_in serveraddr;
    int server_sockfd;
    int client_len;
    char buf[MAXLINE];

    server_sockfd = make_server_sockfd();
    if(-1 == server_sockfd)
    {
        perror("[ERROR] :");
        return -1;
    }

    char addr[] = "127.0.0.1";
    
    int port;
    if (isargentered(argv[1])) port = atoi(argv[1]);
    else port = 3500;
    printf("%d\n",port);
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

    if (-1 == bufferToServer(server_sockfd, buf)) return -1;

    memset(buf,0x00,MAXLINE);
    if (-1 == serverToBuffer(server_sockfd, buf)) return -1;

    close(server_sockfd);
    printf("read : %s\n",buf);
    return 0;
}
