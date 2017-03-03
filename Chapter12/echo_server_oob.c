#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/signal.h> // for signal const
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../err.h"
#define MAXLINE 1024
int client_sockfd;
void urg_handler(int signo) // handler, SIGURG will call this
{
    char buf;
    recv(client_sockfd, (void*)&buf,sizeof(buf), MSG_OOB);
    printf("OOB DATA %c\n", buf);
}

int main(int argc, char *argv[])
{
    int listenfd;
    int readn;
    socklen_t client_len;
    char buf[MAXLINE];
    struct sockaddr_in clientaddr, serveraddr;

    if(argc !=2) err("Usage : [port name]\n", err_failed);

    client_len = sizeof(clientaddr);

    if(0 > (listenfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))) perr(err_one);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1]));

    bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    listen(listenfd, 5);
    while(1)
    {
        memset(buf, 0x00, MAXLINE);
        client_sockfd
            = accept(listenfd, (struct sockaddr*)&clientaddr,&client_len);
        if(client_sockfd == -1) continue;

        signal(SIGURG, urg_handler); // set signal handler
        fcntl(client_sockfd, F_SETOWN,getpid()); // set signal goes to this proc
        
        while(1)
        {
            if(0 >= (readn = read(client_sockfd, buf, MAXLINE)))
            {
                close(client_sockfd);
                break;
            }
            printf("msg : %s\n", buf);
            if(0 >= write(client_sockfd, buf, strlen(buf)))
            {
                close(client_sockfd);
                perror("[ERROR] : ");
            }
            memset(buf,0x00,MAXLINE);
        }
        
    }
    close(client_sockfd);
    return 0;
}