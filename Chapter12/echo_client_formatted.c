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
    int server_sockfd;
    struct sockaddr_in serveraddr;
    int client_len;
    FILE *sock_fp = NULL;
    char buf[MAXBUF];

    if((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))< 0)
        return 1;
    memset((void*)&serveraddr, 0x00, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(3500);
    
    client_len = sizeof(serveraddr);
    if(err_failed ==
        connect(server_sockfd, (struct sockaddr*)&serveraddr, client_len))
        return -1;
    
    sock_fp = fdopen(server_sockfd, "r+");
    if(sock_fp == NULL) return -1;

    printf("format : nickname, realname, age, message\n");
    fgets(buf,MAXBUF-1,stdin);
    if(feof(sock_fp)!=0 || ferror(sock_fp)!=0)
    {
        printf("closing socket..\n");
        fclose(sock_fp);
        return -1;
    }
    fputs(buf,sock_fp);
    sleep(1);
    
    printf("Message from server:\n");
    while(fgets(buf, MAXBUF-1, sock_fp) > 0)
    {
        printf("%s",buf);
    }
    fclose(sock_fp);
    
    return 1;
}