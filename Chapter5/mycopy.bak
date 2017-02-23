#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#define FAILED 0
const int maxline=512;

int main(int argc, char *argv[])
{
    int source_fd;
    int dest_fd;
    int readn;
    int totaln=0;
    char buf[maxline];

    if(argc!=3) return 1;

    source_fd = open(argv[1], O_RDONLY);
    if (FAILED == source_fd) return 1;

    dest_fd = open(argv[2], O_CREAT|O_EXCL|O_WRONLY,
            S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    
    if(FAILED==dest_fd) return 1;

    if(errno == EEXIST)
    {
        perror("Error ");
        close(dest_fd);
        return 1;
    }

    memset(buf, 0x00, maxline);
    while((readn = read(source_fd,buf,maxline)) >0)
        {
            printf("readn : %d\n", readn);
            totaln+=write(dest_fd,buf,readn);
            memset(buf, 0x00, maxline);
        }
    printf("Total Copy Size : %d\n", totaln);
    return 0;

}
