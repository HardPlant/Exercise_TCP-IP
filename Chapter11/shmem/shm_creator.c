#include <sys/ipc.h> // for IPC consts
#include <sys/shm.h> // for shmem
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../err.h"
int main(int argc, char* argv[])
{
    int shmid;
    int *cal_num;
    void *shared_memory = NULL;
    shmid = shmget((key_t)1234, sizeof(int),IPC_CREAT|0666);
    
    if(err_failed == shmid) perr(err_failed);
    
    shared_memory = shmat(shmid, NULL, 0); // shmat(iShmid, vpShmaddr, iShmflg);
    if((void*)err_failed == shared_memory) perr(err_failed);

    cal_num = (int*)shared_memory;
    while(1)
    {
        *cal_num = *cal_num + 2;
        sleep(1);
    }

    stmdt(shared_memory); // shmdt(vpShmaddr);
    //int shmctl(int shmid, int cmd, struct shmid_ds *buf);
    //cmd : [IPC_STAT| IPC_SET | IPC_RMID ]
    //buf : for IPC_STAT, information saved in buf
    
    return 0;
}
