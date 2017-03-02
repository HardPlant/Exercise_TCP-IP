#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> // for shared memory
#include <stdio.h>

int main(int argc, char *argv[])
{
    key_t keyval = 2345;
    int shmid;
    shmid = shmget(keyval, 1024, IPC_CREAT|0666); // get sm by (key, size, auth)
    if (shmid == -1) // "ipcs" shell command to get infos
    {                // and attach bt shmat(), delete by shmdt(), control by shmctl()
        printf("shm create failed\n");
        return 1;
    }
    printf("shm create successed\n");
    return 0;
}