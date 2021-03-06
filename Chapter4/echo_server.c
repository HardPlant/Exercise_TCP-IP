#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXBUF 1024 // same length with client

int main(int argc, char **argv)
{
    int server_sockfd, client_sockfd;
    socklen_t client_len;//not int
    int n;

    char buf[MAXBUF];

    struct sockaddr_in clientaddr, serveraddr;

    client_len = sizeof(clientaddr);

    server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(-1 == server_sockfd)
    {
        perror("[ERROR] SOCKET :");
        exit(0);
    }
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1]));

    bind(server_sockfd, (struct sockaddr*) &serveraddr
            ,sizeof(serveraddr));
    listen(server_sockfd, 5);

    while(1)
    {
        memset(buf,0x00,MAXBUF);
        client_sockfd = accept(server_sockfd
                ,(struct sockaddr*) &clientaddr, &client_len);
        printf("New Client Connect: %s\n"
                ,inet_ntoa(clientaddr.sin_addr));
        n = read(client_sockfd, buf, MAXBUF);
        if(n <= 0)
        {
            close(client_sockfd);
            continue;
        }
        if(write(client_sockfd, buf, MAXBUF) <= 0)
        {
            perror("[ERROR] Write:");
            close(client_sockfd);
        }
        printf("write:%s\n",buf);
        close(client_sockfd);
    }
    close(server_sockfd);
    return 0;

}

