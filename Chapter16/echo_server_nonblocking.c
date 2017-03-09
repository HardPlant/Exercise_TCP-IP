#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include "../err.h"
#define MAXLINE 1024
/* will cause busy wait */
int set_nonblock_socket(int fd)
{/* make nonblock */
    int flags;
    if((flags = fcntl(fd, F_GETFL,0)) == err_failed) // get fd status
        perr(err_failed);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK); // fcntl(), add O_NONBLOCK
    return 0;
}

int main(int argc, char *argv[])
{
    int server_sockfd, client_sockfd;
    socklen_t client_len;
    int n;
    char buf[MAXLINE];
    struct sockaddr_in clientaddr, serveraddr;
    int client_fd_array[1024];
    int maxfd = 0;
    int i = 0;
    memset((void*)client_fd_array, -1, sizeof(int)*1024); // make 1024 fds
    client_len = sizeof(clientaddr);        // 1 means connected
    if((server_sockfd = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP)) <0)
        perr(err_failed);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1]));

    if(bind(server_sockfd, (struct sockaddr*)&serveraddr,
        sizeof(serveraddr)) == err_failed)
        perr(err_failed);
    if(listen(server_sockfd, 5) == -1) perr(err_failed);
/* server : socket -> bind -> listen*/
    set_nonblock_socket(server_sockfd); // now accept() return immediately
    maxfd = server_sockfd;
    while(1)
    {
        memset(buf, 0x00, MAXLINE);
        client_sockfd = accept(server_sockfd,
            (struct sockaddr*)&clientaddr, &client_len);
        if(client_sockfd == err_failed) // check if error
        {
            if(errno == EAGAIN) // no error
            {
                
            }
            else
            {
                printf("Error %d\n", errno);
                continue;
            }
        }
        else{
            set_nonblock_socket(client_sockfd);
            client_fd_array[client_sockfd] = 1;
            if(client_sockfd > maxfd)
                maxfd = client_sockfd;
            continue;
        }
        memset(buf, 0x00, sizeof(buf));
        for(i=0; i<maxfd + 1; i++) // check incoming data
        {
            if(client_fd_array[i] != 1) continue;
            client_sockfd = i;
            n = read(client_sockfd, buf, MAXLINE);
            if(n==-1)
            {
                if(errno == EAGAIN)
                {

                }
                else{
                    printf("read error : %d\n", errno);
                    close(client_sockfd);
                    client_fd_array[client_sockfd] = -1;
                    break;
                }
            }
            else if(n == 0)
            {
                printf("close %d\n", errno);
                close(client_sockfd);
                client_fd_array[client_sockfd] = -1;
                break;
            }
            else
            {
                printf("Read data %s\n", buf);
                write(client_sockfd, buf, MAXLINE);
            }
        }
    }
}