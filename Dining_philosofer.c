#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

pthread_mutex_t forks[N];   // 🔥 FIXED HERE
sem_t room;

void* philosopher(void* arg)
{
    int id = *(int*)arg - 1;

    while(1)
    {
        printf("Philosopher %d is thinking\n", id+1);
        sleep(1);

        sem_wait(&room);

        pthread_mutex_lock(&forks[id]);
        pthread_mutex_lock(&forks[(id+1)%N]);

        printf("Philosopher %d is eating\n", id+1);
        sleep(2);

        pthread_mutex_unlock(&forks[id]);
        pthread_mutex_unlock(&forks[(id+1)%N]);

        sem_post(&room);

        printf("Philosopher %d finished eating\n", id+1);
        sleep(1);
    }
}

int main()
{
    pthread_t p[N];
    int id[N];

    sem_init(&room, 0, N-1);

    for(int i=0;i<N;i++)
        pthread_mutex_init(&forks[i], NULL);

    for(int i=0;i<N;i++)
    {
        id[i] = i+1;
        pthread_create(&p[i], NULL, philosopher, &id[i]);
    }

    for(int i=0;i<N;i++)
        pthread_join(p[i], NULL);

    return 0;
}
