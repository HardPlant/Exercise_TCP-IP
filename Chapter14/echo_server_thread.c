#include <pthread.h> // include -lpthread
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../err.h"
#define MAXLINE 1024
#define PORTNUM 3600

void *thread_func(void *data) // get client_fd to data
{// read data.
    int sockfd = *((int*)data);
    int readn;
    socklen_t addrlen;
    char buf[MAXLINE];
    struct sockaddr_in client_addr;
    memset(buf, 0x00, MAXLINE);
    addrlen = sizeof(client_addr);
    getpeername(sockfd, (struct sockaddr*)&client_addr, &addrlen);
    while((readn = read(sockfd, buf, MAXLINE)) > 0)
    {
        printf("Read Data %s(%d) : %s"
            , inet_ntoa(client_addr.sin_addr)
            , ntohs(client_addr.sin_port)
            , buf);
        write(sockfd, buf, strlen(buf));
        memset(buf, 0x00, MAXLINE);
    }
    close(sockfd);
    printf("worker thread end\n");
    return 0;
}
int main(int argc, char *argv[])
{
    int listen_fd, client_fd;
    socklen_t addrlen;
    pthread_t thread_id;

    struct sockaddr_in server_addr, client_addr;

    if((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <0)
        perr(err_failed);
    memset((void*)&server_addr, 0x00, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORTNUM);

    if(bind(listen_fd, (struct sockaddr*)&server_addr,
            sizeof(server_addr)) == err_failed)
        perr(err_failed);

    if(listen(listen_fd, 5) == err_failed)
        perr(err_failed);

    while(1)
    {
        addrlen = sizeof(client_addr);
        client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addrlen);
        if(client_fd == err_failed)
            perr(err_failed);
        else
        {//create read thread
            pthread_create(&thread_id, NULL, thread_func, (void*)&client_fd);
            pthread_detach(thread_id);
        }
    }
    return 0;
    
}