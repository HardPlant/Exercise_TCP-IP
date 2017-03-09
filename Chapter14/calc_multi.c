#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../err.h"
#define ARRAY_SIZE 100
#define THREAD_NUM 4

pthread_mutex_t t_lock;
pthread_cond_t t_cond;

int *data_array;
int sum_array[THREAD_NUM];

struct data_info
{
    int *d_point; // addr of data_array
    int idx; // which thread
};

void *t_func(void *data)
{
    struct data_info d_info;
    int i = 0;
    int sum = 0;
    d_info = *((struct data_info*)data);

    pthread_mutex_lock(&t_lock);
    pthread_cond_wait(&t_cond, &t_lock); // wait for signal
    printf("Start %d thread\n", d_info.idx);
    pthread_mutex_unlock(&t_lock);

    for(i=0; i<25; i++)
    {
        sum += d_info.d_point[(d_info.idx*25)+i]; // add allocated 25 data
    }

    printf("(%d) %d\n", d_info.idx, sum);
    sum_array[d_info.idx] = sum; // save datas

    return NULL; // not void(), void*() (returns addr)
}

int main(int argc, char *argv[])
{
    int i=0;
    int sum=0;
    struct data_info d_info;

    pthread_t thread_id[THREAD_NUM];

    if((data_array = malloc(sizeof(int)*ARRAY_SIZE)) == NULL)
        perr(err_failed);

    pthread_mutex_init(&t_lock, NULL);
    pthread_cond_init(&t_cond, NULL);

    for(i = 0; i < THREAD_NUM; i++)
    {
        d_info.d_point = data_array;
        d_info.idx = i;

        pthread_create(&thread_id[i], NULL, t_func, (void*)&d_info);
        usleep(100);
    }

    for(i=0; i<ARRAY_SIZE; i++)
    { // fill data to calculate
        *data_array = i;
        data_array++;
    }

    pthread_cond_broadcast(&t_cond); // awake all thread
    for(i=0; i<THREAD_NUM; i++)
    {
        pthread_join(thread_id[i], NULL);
        sum += sum_array[i];// if worker thread exits, fetch data from array.
    }
    printf("SUM (0-99) L %d\n",sum);
    return 0;
}