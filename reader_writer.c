#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int readcount = 0;

sem_t mutex;   // protects readcount
sem_t wrt;     // controls writer access

void* reader(void* arg)
{
    int id = *(int*)arg;

    while(1)
    {
        sem_wait(&mutex);
        readcount++;

        if(readcount == 1)
            sem_wait(&wrt);   // first reader blocks writer

        sem_post(&mutex);

        printf("Reader %d is reading\n", id);
        sleep(1);

        sem_wait(&mutex);
        readcount--;

        if(readcount == 0)
            sem_post(&wrt);   // last reader releases writer

        sem_post(&mutex);

        sleep(1);
    }
}

void* writer(void* arg)
{
    int id = *(int*)arg;

    while(1)
    {
        sem_wait(&wrt);

        printf("Writer %d is writing\n", id);
        sleep(2);

        sem_post(&wrt);

        sleep(1);
    }
}

int main()
{
    pthread_t r[5], w[5];
    int id[5];

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    for(int i=0;i<5;i++)
    {
        id[i] = i+1;

        pthread_create(&r[i], NULL, reader, &id[i]);
        pthread_create(&w[i], NULL, writer, &id[i]);
    }

    for(int i=0;i<5;i++)
    {
        pthread_join(r[i], NULL);
        pthread_join(w[i], NULL);
    }

    return 0;
}
