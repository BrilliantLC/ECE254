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
#include <sys/stat.h>
#include <stdbool.h>
#include <pthread.h>

sem_t lock, spaces, items, ndone;

int *buffer;

// producer and consumer counter
int pcount = 0;
int ccount = 0;

int n, b, p, c;

struct producerid
{
    int item;
};
struct consumerid
{
    int item;
};

void *producer(void *id)
{
    // extract producer id
    struct producerid *elem = id;
    int elemv = elem->item;

    for (int i = 0; i < n; i++)
    {
        if (i % p == elemv)
        {
            sem_wait(&spaces);

            // write buffer
            sem_wait(&lock);
            buffer[pcount] = i;
            pcount++;
            sem_post(&lock);

            sem_post(&items);
        }
    }
}

void *consumer(void *cid)
{
    int elem;
    int sr;

    // extract consumer id
    struct consumerid *elemid = cid;
    int id = elemid->item;

    // while number of items consumed is less than n
    while (ccount < n)
    {
        // break out of the loop if consumed all items,
        // this solves the deadlock
        sem_wait(&ndone);
        if (ccount == n)
        {
            sem_post(&ndone);
            break;
        }

        sem_wait(&items);

        // read from buffer
        sem_wait(&lock);
        int elem = buffer[ccount];
        ccount++;
        // calculate square root
        sr = sqrt(elem);
        if (sr * sr == elem)
        {
            printf("%d %d %d\n", id, elem, sr);
        }
        sem_post(&lock);

        sem_post(&spaces);
        sem_post(&ndone);
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
    sem_init(&ndone, 0, 1);

    // setup and start timer
    struct timeval tv;
    double t1;
    double t2;

    gettimeofday(&tv, NULL);
    t1 = tv.tv_sec + tv.tv_usec / 1000000.0;

    // initialize the buffer and allocate memory
    free(buffer);
    buffer = malloc(sizeof(int) * n);

    pthread_t producers[p], consumers[c];

    int id, cid;

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

    // create POSIX threads for p producers and c consumers
    for (id = 0; id < p; id++)
    {
        pthread_create(&producers[id], NULL, producer, (void *)&pcid[id]);
    }
    for (cid = 0; cid < c; cid++)
        pthread_create(&consumers[cid], NULL, consumer, (void *)&cmid[cid]);

    // wait for all producers and consumers to finish
    for (id = 0; id < p; id++)
        pthread_join(producers[id], NULL);
    for (cid = 0; cid < c; cid++)
        pthread_join(consumers[cid], NULL);

    // calculate and show execution time
    gettimeofday(&tv, NULL);
    t2 = tv.tv_sec + tv.tv_usec / 1000000.0;
    printf("System execution time: %.6lf seconds\n", t2 - t1);

    // free up memory used by the buffer
    free(buffer);
    return 0;
}