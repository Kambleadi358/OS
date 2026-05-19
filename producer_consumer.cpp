#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void* producer(void* arg)
{
    int id = *((int*)arg);

    for(int i=0;i<1;i++)
    {
        int item = rand() % 100;

        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = item;
        cout << "Producer " << id << " produce " << item << " in buffer" << endl;
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);
    }

    pthread_exit(0);
}

void* consumer(void* arg)
{
    int id = *((int*)arg);

    for(int i=0;i<1;i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);

        int item = buffer[out];
        cout << "Consumer " << id << " consume " << item << " from buffer" << endl;
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);
    }

    pthread_exit(0);
}

int main()
{
    pthread_t p[5], c[5];
    int pid[5], cid[5];

    sem_init(&empty,0,BUFFER_SIZE);
    sem_init(&full,0,0);
    sem_init(&mutex,0,1);

    srand(time(0));

    for(int i=0;i<5;i++)
    {
        pid[i] = i+1;
        pthread_create(&p[i],NULL,producer,&pid[i]);
    }

    for(int i=0;i<5;i++)
    {
        cid[i] = i+1;
        pthread_create(&c[i],NULL,consumer,&cid[i]);
    }

    for(int i=0;i<5;i++)
        pthread_join(p[i],NULL);

    for(int i=0;i<5;i++)
        pthread_join(c[i],NULL);

    return 0;
}
