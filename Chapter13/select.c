#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../err.h"
#define STDIN 0
#define MAXLINE 80

int main(int argc, char *argv[])
{
    fd_set readfds; // make fd set
    int fdn;
    char rbuf[MAXLINE];
    struct timeval timeout;

    FD_ZERO(&readfds); //fill all fields to 0
    FD_SET(STDIN, &readfds); // add fdnum to set
                            // and if fd modified flag will be set to 1. 

    while(1)
    {
        timeout.tv_sec = (long)5;
        timeout.tv_usec = 0L;
        printf("> ");
        fflush(stdout);
        fdn = select(STDIN+1, &readfds, NULL, NULL, &timeout);
        if(fdn == 0) printf("\n[ERROR] Time Out\n");
        else
        {
            memset(rbuf, 0x00, MAXLINE);
            read(STDIN, rbuf, MAXLINE -1);
            if(strncmp(rbuf, "quit\n", 5) == 0) break;
            printf("Your Message : %s", rbuf);
        }
        FD_SET(STDIN, &readfds);
    }
    return 0;
}