#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "../err.h"

int main(int argc, char *argv[])
{
    int rfd, wfd;
    const int maxline = 1024;
    char buf[maxline];
    const char* readpipe= "/tmp/myfifo_r";
    const char* writepipe= "/tmp/myfifo_w";

    mkfifo(readpipe, S_IRUSR|S_IWUSR);
    mkfifo(writepipe, S_IRUSR|S_IWUSR);
    
    rfd = open(readpipe, O_RDWR);
    if(err_failed == rfd) perr(err_failed);

    wfd = open(writepipe, O_RDWR);
    if (err_failed == wfd) perr(err_failed);

    while(1)
    {
        memset(buf,0x00,maxline);
        if(read(rfd, buf, maxline) < 0) err("Read Error", err_failed);

        printf("Read : %s", buf);
        write(wfd, buf, maxline);
        lseek(wfd,0,SEEK_SET); // flush buffer
    }
    return 0;
}