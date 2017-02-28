#include <sys/types.h>
#include <sys/stat.h>

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
    
    const char* readpipe= "/tmp/myfifo_w"; //connect cli_read -> sv_write
    const char* writepipe= "/tmp/myfifo_r"; //connect cli_write->sv_read

    rfd = open(readpipe, O_RDWR);
    if(err_failed == rfd) perr(err_failed);

    wfd = open(writepipe, O_RDWR);
    if (err_failed == wfd) perr(err_failed);

    while(1)
    {
        printf("> ");
        fflush(stdout); //flushs stdout, required substitute
        memset(buf, 0x00, maxline);
        if(read(0, buf, maxline) < 0) err("err", err_failed); // read from stdin(0)
        
        if(strncmp(buf, "quit\n", 5) == 0 ) break;

        write(wfd, buf, strlen(buf));
        read(rfd, buf, maxline);
        printf("Server -> %s", buf);
    }
    close(wfd);
    close(rfd);
    return 0;
}