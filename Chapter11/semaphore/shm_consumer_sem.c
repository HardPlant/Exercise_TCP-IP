#include <sys/ipc.h> // for IPC consts
#include <sys/shm.h> // for shmem
#include <sys/sem.h> // for semaphore
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../err.h"
int main(int argc, char* argv[])
{
    int shmid;
    int semid;

    struct sembuf semopen = {0, -1, SEM_UNDO};// SEM_UNDO will automatically
    struct sembuf semclose = {0, 1, SEM_UNDO}; // return sem at terminate time

    int *cal_num;
    void *shared_memory = NULL;
    shmid = shmget((key_t)1234, sizeof(int),IPC_CREAT|0666);
    
    if(err_failed == shmid) perr(err_failed);
    
    semid = semget((key_t)3477, 0, 0666);
    if(err_failed == semid) perr(err_failed);

    shared_memory = shmat(shmid, NULL, 0); // shmat(iShmid, vpShmaddr, iShmflg);
    if((void*)err_failed == shared_memory) perr(err_failed);

    cal_num = (int*)shared_memory;
    while(1)
    {
        int local_var = 0;
        if(semop(semid, &semopen,1) == err_failed) perr(err_failed);

        local_var = *cal_num + 1;
        sleep(2);
        *cal_num = local_var;
        printf("count %d\n", *cal_num);

        semop(semid, &semclose,1);
    }

    return 1;
}
