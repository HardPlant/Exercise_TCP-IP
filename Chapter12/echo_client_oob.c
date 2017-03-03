#include <sys/socket.h> // socket functions
#include <arpa/inet.h> // for socket support
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024
int make_server_sockfd()
{
    return socket(AF_INET,SOCK_STREAM,0);
}
int bufferToServer(int server_sockfd, char* buf)
{
    static int i=1;
    read(0,buf,MAXLINE);
    if (i%3 == 0)
    {
        send(server_sockfd, buf,sizeof(buf), MSG_OOB);
    }
    else if (write(server_sockfd, buf, MAXLINE) <= 0)
    {
        perror("[ERROR] WRITE:");
        return -1;
    }
    i++;
    return 0;
}
int serverToBuffer(int server_sockfd, char* buf)
{
    memset(buf,0x00,MAXLINE);
    if(read(server_sockfd, buf, MAXLINE <= 0))
    {
        perror("[ERROR] READ:");
        return -1;
    }
    return 0;
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
    int port = 3500;
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
while(1)
{
    memset(buf,0x00,MAXLINE);
    if (-1 == bufferToServer(server_sockfd, buf)) return -1;

    memset(buf,0x00,MAXLINE);
    if (-1 == serverToBuffer(server_sockfd, buf)) return -1;

    printf("read : %s",buf);
}
    close(server_sockfd);
    return 0;
}
