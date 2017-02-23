#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int maxline = 1024;
void err()
{
    perror("[ERROR] ");
    exit(1);
}
int main(int argc, char **argv)
{
    struct sockaddr_in serveraddr;
    int server_sockfd;
    int client_len;
    char buf[maxline];
    char rbuf[maxline];

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sockfd < 0) err();

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = 3500; // err : 44045, htons (2byte)

    client_len = sizeof(serveraddr);
    if(connect(server_sockfd, (struct sockaddr*)&serveraddr,
                client_len) <0)
    {
        err();
    }
    memset(buf, 0x00,maxline);
    read(stdin, buf, maxline);
    if(write(server_sockfd, buf, maxline) <=0) err();

    memset(buf, 0x00, maxline);
    if(read(server_sockfd, buf, maxline) <=0) err();

    close(server_sockfd);
    printf("read : %s", buf);

    return 0;

}
