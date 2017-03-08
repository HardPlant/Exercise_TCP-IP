#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXTHREAD 2
/* -lpthread to link pthread library, or link error occurs.*/
void *t_func(void *data) // increase count.
{
    int *count = (int*)data;
    int tmp;
    pthread_t thread_id = pthread_self(); // get pthread_create() id

    while(1)
    {
        printf("%lu %d\n", thread_id, *count);
        *count = *count + 1;
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    pthread_t thread_id[MAXTHREAD];
    int i=0;
    int count = 0;

    for(i=0; i<MAXTHREAD; i++)
    {
        pthread_create(&thread_id[i], NULL, t_func, (void*)&count);
        usleep(5000);
    }

    while(1)
    {
        printf("Main Thread : %d\n", count);
        sleep(2);
    }

    for(i=0; i<MAXTHREAD;i++)
    {
        pthread_join(thread_id[i], NULL); // will not executed, note for flow.
    }
    return 0;
}