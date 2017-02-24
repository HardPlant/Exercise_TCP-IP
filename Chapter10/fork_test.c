#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../err.h"
int main(int argc, char *argv[])
{
    pid_t pid;
    int i=100;

    printf ("Prigram Start!\n");
    pid = fork();        /// *Copies* Process (do not Create)
    printf("forked!\n"); //| pid < 0 : failed
                         //| pid == 0 : Suceed to Child Process
                         //\ pid > 0 : Succed to Parent, returns pid
    if(pid < 0) err("fork failed!\n", err_fail);
    if(pid ==0)
    {                                                           
        printf("I'm Parent Proc %d\n", getpid());           //   <-\ Executes next  
        i++;                                                //      |           |
        sleep(1);                                           //      |           |
    }                                                       //      |           |
    else if(pid>0)                                          //      |           |
    {                                               //              |           |
        printf("I'm Child Proc %d\n", getpid()); // Executes First  |           |
        while(1)                                            //      |then loop<-/
        {                                                   //      |
            printf("C : %d\n", i);                          //      |
            i+=2;                                           //      |
            sleep(1); // First Sleep -> go to Parent            ----/
        }
    }
}              