#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "../err.h"

#define MAXLINE 1024
#define PORT 3500

int main(int argc, char *argv[])
{
    int listen_fd, client_fd;
    pid_t pid; //
    socklen_t addrlen;
    int readn;
    char buf[MAXLINE];
    struct sockaddr_in client_addr, server_addr;

    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(0 > listen_fd) perr(err_one);

    memset((void*)&server_addr, 0x00, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if(err_failed == bind(listen_fd, (struct sockaddr*)&server_addr,
                    sizeof(server_addr)))
        perr(err_one);
    
    if(err_failed == listen(listen_fd, 5))
        perr(err_one);
    //socket() -> bind() -> listen()
    signal(SIGCHLD, SIG_IGN); // ignores(IGN) Child Terminate Signal(SIGCHLD)
    while(1)
    {
        addrlen = sizeof(client_addr);
        client_fd = accept(listen_fd,
                        (struct sockaddr*)&client_addr, &addrlen);
        if(err_failed == client_fd)
        {
            printf("accept failed\n"); // while loop, don't use exit(retn);
            break;
        }

        pid = fork(); // create child!
        if(pid == 0) // if child
        {
            memset(buf, 0x00, MAXLINE);
            while((readn = read(client_fd, buf, MAXLINE))>0)
            {
                printf("Read Data %s(%d) : %s\n",
                    inet_ntoa(client_addr.sin_addr),
                    client_addr.sin_port,
                    buf);
                write(client_fd, buf, strlen(buf));
                memset(buf,0x00,MAXLINE);
            }
            close(client_fd);
            return 0;
        }
        
        else if (pid > 0)
            close(client_fd);
    }
    return 0;
}