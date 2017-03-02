#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../err.h"
int main(int argc, char *argv[])
{
    int shmid;
    int *cal_num;
    void *shared_memory = NULL;

    shmid = shmget((key_t)1234, sizeof(int),0); // shmget(key_t key, iSize, iShmflg);
    
    if(err_failed == shmid) perr(err_failed);

    shared_memory = shmat(shmid, NULL, 0); // shmat(iShmid, pvShmaddr, iShmflg);
    if((void*)err_failed == shared_memory) perr(err_failed);

    cal_num = (int*) shared_memory;

    while(1)
    {
        sleep(1);
        printf("Read data : %d\n", *cal_num);
    }
    return 1;
}