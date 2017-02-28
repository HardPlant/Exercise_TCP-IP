#include <unistd.h> // for pipe, simplex
#include <stdlib.h> // \- int pipe(int filedes[2]);
#include <stdio.h>
#include <string.h>
#include "../err.h"
int main()
{
    const int ReadPipe = 0;
    const int SendPipe = 1;
    int fd[2]; // write <-> \n read <->
    int buf;
    int i=0;
    int pid;

    if (pipe(fd) < 0) perr(err_one); // 0 -> readOnly, 1-> writeOnly

    pid = fork();
    if(pid < 0) perr(err_one);

    else if (pid == 0) // child
    {
        close(fd[ReadPipe]);
        while(1) // write int
        {
            i++;
            write(fd[SendPipe], (void*)&i,sizeof(i));
            sleep(1);
        }
    }

    else //parent
    {
        close(fd[SendPipe]);
        while(1)
        {
            read(fd[ReadPipe],(void*)&buf, sizeof(buf));
            printf("> %d\n", buf);
        }
    }
    return 1;
}
