#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h> //for semaphore
#include <string.h> //  * semget(key_t key, iNsems, [IPC_CREAT|IPC_EXCL]);
#include <unistd.h> //  * semtl()
#include <stdio.h>  //  * semop(iSemid, struct sembuf *sops, unsigned nsops);
#include <stdlib.h>                     // \-* {short sem_num,sem_op,sem_flg};
                                                            //\-* number of sems
                                        // * sem_num: which sem in sem array
#include "../../err.h"                  // * sem_ip : -1 for get locked, 1 for unlock
                                        // * sem_flg : [IPC_NOWAIT | SEM_UNDO]
union semun
{
    int val;
};

int main(int argc, char *argv[])
{
    int shmid;
    int semid;

    int *cal_num;
    void *shared_memory = NULL;
    union semun sem_union;
    struct sembuf semopen = {0, -1, SEM_UNDO};// SEM_UNDO will automatically
    struct sembuf semclose = {0, 1, SEM_UNDO}; // return sem at terminate time

    shmid = shmget((key_t)1234, sizeof(int),0); // shmget(key_t key, iSize, iShmflg);
    if(err_failed == shmid) perr(err_failed);

    semid = semget((key_t)3477, 1, IPC_CREAT|0666); // 1 for one sem set

    if(err_failed == semid) perr(err_failed);

    shared_memory = shmat(shmid, NULL, 0); // shmat(iShmid, pvShmaddr, iShmflg);
    if((void*)err_failed == shared_memory) perr(err_failed);

    cal_num = (int*) shared_memory;

    sem_union.val = 1;
    if(err_failed == semctl(semid, 0, SETVAL, sem_union)); // SETVAL: semctl cmd

    while(1)
    {
        int local_var = 0;
        if (err_failed == semop(semid, &semopen, 1)) perr(err_one);
        local_var = *cal_num +1;
        sleep(1);
        *cal_num = local_var;
        printf("Read data : %d\n", *cal_num);
        semop(semid, &semclose, 1); // 1 for number of sem union, 
    }
    return 1;
}