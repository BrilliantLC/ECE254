//multiple thread implementation
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>
#include "circular_buffer.h"
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

double ta, tb;
struct timeval tv;

int n, b, p, c, count;

CircularBuffer buffer;
ElemType elem;

sem_t producer_wait, consumer_wait;

void producer(void *id) {
	int i;

	for( i = 0; i < n; i++ ) {
		if( i % p == id ) {
			sem_wait(&producer_wait);

			elem.value = i;
			cbWrite(&buffer, &elem);		

			sem_post(&consumer_wait);
		}	
	}
	//free(id);
	pthread_exit(0);
}

void consumer(void *id) {
	int sq;

	while( count < n ) {
		sem_wait(&consumer_wait);
		
		cbRead(&buffer, &elem);
		count++;

		sem_post(&producer_wait);

		sq = sqrt(elem.value);
		if(sq*sq == elem.value)
		{
			printf("c%d %d %d\n", id, sq, elem.value);
		}
	}
	sem_post(&consumer_wait);
	//free(id);
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	n = atoi(argv[1]);
	b = atoi(argv[2]);
	p = atoi(argv[3]);
	c = atoi(argv[4]);
	count = 0;
	
	sem_init(&consumer_wait, 0, 0);
	sem_init(&producer_wait, 0, b);

	cbInit(&buffer, b);
	
	//calculating and passing ta
	gettimeofday(&tv, NULL);
	ta = tv.tv_sec + tv.tv_usec/1000000.0;

	int i, j;
	pthread_t producers[p], consumers[c];

	for( i = 0; i < p; i++) {
        pthread_create(&producers[i], NULL, producer, (int*)i);
	}

	for(j = 0; j < c; j++) {
        pthread_create(&consumers[j], NULL, consumer, (int*)j);
	}
	
	for( i = 0; i < p; i++)	{
		pthread_join(producers[i], NULL);
	}

	for(j = 0;j < c; j++) {
		pthread_join(consumers[j], NULL);
	}
	
	//calculating and passing tb
	gettimeofday(&tv, NULL);
	tb = tv.tv_sec + tv.tv_usec/1000000.0;

	printf("System execution time: %.6lf seconds\n", tb-ta);

	cbFree(&buffer);
	return 0;
}