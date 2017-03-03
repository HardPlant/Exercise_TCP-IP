#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <stdlib.h>
#include "../err.h"
#define MAXBUF 256

int main(int argc, char *argv[])
{
    int server_sockfd, client_sockfd;
    socklen_t client_len;
    char buf[MAXBUF];
    struct sockaddr_in clientaddr, serveraddr;
    FILE *sock_fp = NULL;

    client_len = sizeof(clientaddr);

    if((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))< 0)
        perr(err_one);

    memset((void*)&serveraddr, 0x00, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serveraddr.sin_port =htons(atoi(argv[1]));

    bind(server_sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(server_sockfd, 5);

    while(1)
    {
        client_sockfd
         = accept(server_sockfd, (struct sockaddr*)&clientaddr, &client_len);
        sock_fp = fdopen(client_sockfd, "r+");
        if(sock_fp == NULL) continue;
        while(1)
        { // no need to memset();
            if(fgets(buf, MAXBUF-1, sock_fp) == NULL) // instead of read()
            {
                printf("closing socket..\n");
                fclose(sock_fp);
                break;
            }
            if(fputs(buf, sock_fp) == -1) // instead of write()
            {
                printf("closing socket..\n");
                fclose(sock_fp);
                break;
            }
        }
    }
    return 1;
}