#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../err.h"
/*
fork() -> create child, make pid
execl() -> overwrite current pid
fork()+execl()!?
*/
#define MAXLINE 256
#define PROMPT "$ "
#define chop(str) str[strlen(str) -1] = 0x00;

int main(int argc, char **argv)
{
    char buf[MAXLINE];
    int proc_status;
    pid_t pid;
    printf("My Shell Ver 1.0\n");
    while(1)
    {
        printf("%s", PROMPT);
        memset(buf, 0x00, MAXLINE);
        fgets(buf, MAXLINE -1, stdin);
        if(strncmp(buf, "quit\n", 5) == 0) break;

        chop(buf);
        pid = fork();
        if(pid == 0)
        {
            if(execl(buf, buf, NULL) == -1) err("Execl failed\n", err_fail);
        }
        if(pid >0)
        {
            printf("Child wait\n");
            wait(&proc_status);
            printf("Child exited\n");
        }
    }
    return 0;
}
