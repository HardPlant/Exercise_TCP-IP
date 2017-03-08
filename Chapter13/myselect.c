#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include "../err.h"
#define MAXLINE 1024
/* check two pipe, read if data enters*/
void ReadLine(int maxfd, fd_set *readfds);

int main(int argc, char* argv[])
{
    int tfd[2];
    int i = 0;
    int maxfd = 0;
    int fdn = 0;
    fd_set readfds;

    if (argc != 3) err("%s [file 1] [file 2]\n", err_one);
    
    FD_ZERO(&readfds); // check if data exist
    for(i=0;i<2;i++)
    {
        printf("Open %s\n", argv[i+1]);
        tfd[i] = open(argv[i+1], O_RDWR); //open named pipe, set biggest fd
        if(tfd[i] == -1) perr(err_one);
        if (tfd[i] > maxfd)
            maxfd = tfd[i];
        FD_SET(tfd[i], &readfds); //adds fd with fd_set macro
    }

    printf("Max fd is %d\n", maxfd);
    while(1)
    {
        fdn = select(maxfd+1, &readfds, NULL, NULL, NULL);
                    //(nfds, readfds, writefds, exceptfds, timeout)
                    // check all fd. be care of maxfd+1(number of fds)
        ReadLine(maxfd+1,&readfds);
    }
    return 0;
}

void ReadLine(int maxfd, fd_set *readfds)
{
    char buf[MAXLINE];
    int i=0;
    for(i=0; i<maxfd;i++)
    {
        if(FD_ISSET(i, readfds))
        {
            while(1)
            {
                memset(buf, 0x00, MAXLINE);
                if(read(i, buf, MAXLINE-1) < MAXLINE-1) break;
                printf("'(%d)->%s'", i, buf);
            }
            printf("(%d)->%s",i,buf);
        }
        FD_SET(i, readfds); // use FD_SET macro    
    }
    
}