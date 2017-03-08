#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXTHREAD 2
/* -lpthread to link pthread library, or link error occurs.*/
pthread_mutex_t m_lock;

void *t_func(void *data) // increase count.
{
    int *count = (int*)data;
    int tmp;
    pthread_t thread_id = pthread_self(); // get pthread_create() id

    while(1)
    {
        pthread_mutex_lock(&m_lock);
        tmp = *count;
        tmp++;
        sleep(1);
        *count = tmp;
        printf("%lu %d\n", thread_id, *count);
        pthread_mutex_unlock(&m_lock);
    }
}

int main(int argc, char *argv[])
{
    pthread_t thread_id[MAXTHREAD];
    int i=0;
    int count = 0;
    // init mutex
    if(pthread_mutex_init(&m_lock, NULL) != 0)
    {
        perror("Mutex init failed");
        return 1;
    }

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