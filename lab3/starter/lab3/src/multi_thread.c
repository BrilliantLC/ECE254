#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>

#include <string.h>
#include <sys/stat.h>

#include <stdbool.h>
#include <pthread.h>

sem_t lock;
sem_t spaces;
sem_t items;

int *buffer;
int pcount = 0;
int ccount = 0;

int n, b, p, c;

struct producerid{int item;};
struct consumerid{int item;};

void *producer(void *id)
{
    struct producerid *elem = id;
    int elemv = elem->item;
    printf("elemv:%d, n:%d\n", elemv, n);
    for (int i = 0; i < n; i++)
    {
        printf("i:%d\n",i);
        if (i % p == elemv)
        {   
            sem_wait(&spaces);

            sem_wait(&lock);
            buffer[pcount] = i;
            printf("added %d\n", i);
            pcount++;
            printf("pcount is:%d\n",pcount);

            sem_post(&lock);

            sem_post(&items);
        }
    }
    // add all corresponding integers to buffer
    // printf(elemv);
    // while (elem < n)
    // {
    //     printf("123");
    //     sem_wait(&spaces);

    //     sem_wait(&lock);
    //     buffer[pcount] = elemv;
    //     pcount++;
    //     elem += p;
    //     sem_post(&lock);

    //     sem_post(&items);
    // }
}

void *consumer(void *cid)
{
    int elem;
    int sr;
    // struct consumerid *elem = cid;
    // int id = elem->d
    printf("sup\n");
    while (ccount < n)
    {
        sem_wait(&items);

        sem_wait(&lock);
        int elem = buffer[ccount];
        ccount++;
        sr = sqrt(elem);
        if (sr * sr == elem)
        {
            printf("%p %d %d\n", (int *)cid, elem, sr);
        }
        sem_post(&lock);

        sem_post(&spaces);
    }
}

int main(int argc, char *argv[])
{
    // check number of arguments
    if (argc != 5)
    {
        printf("Invalid number of arguments!\n");
        exit(1);
    }

    // read input arguments
    n = atoi(argv[1]);
    b = atoi(argv[2]);
    p = atoi(argv[3]);
    c = atoi(argv[4]);

    // initialize semaphores
    sem_init(&lock, 0, 1);
    sem_init(&items, n, 0);
    sem_init(&spaces, 0, b);

    struct timeval tv;
    double t1;
    double t2;

    // start timer
    gettimeofday(&tv, NULL);
    t1 = tv.tv_sec + tv.tv_usec / 1000000.0;

    // initialize the buffer and allocate memory
    free(buffer);
    buffer = malloc(sizeof(int) * n);

    pthread_t producers[p], consumers[c];

    int id, cid;
    printf("y1\n");
    // save id for producer and consumer to be passed to the member functions
    struct producerid pcid[p];
    struct consumerid cmid[c];
    for (id = 0; id < p; id++)
    {
        pcid[id].item = id;
    }
    for (cid = 0; cid < c; cid++)
    {
        cmid[cid].item = cid;
    }
    printf("y2\n");
    // create POSIX threads, p producers and c consumers
    for (id = 0; id < p; id++)
        pthread_create(&producers[id], NULL, producer, (void*) &pcid[id]);
    for (cid = 0; cid < p; cid++)
        pthread_create(&consumers[cid], NULL, consumer, (void*) &cmid[id]);
    printf("y3\n");
    // wait for all producers and consumers to finish
    for (id = 0; id < p; id++)
        pthread_join(producers[id], NULL);
    printf("yyy\n");
    for (cid = 0; cid < c; cid++)
        pthread_join(consumers[cid], NULL);
    printf("y4\n");
    // calculate execution time
    gettimeofday(&tv, NULL);
    t2 = tv.tv_sec + tv.tv_usec / 1000000.0;
    printf("System execution time: %.6lfs.\n", t2 - t1);

    // free up memory used by the buffer
    free(buffer);
    return 0;
}