/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <mqueue.h>
#include <errno.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>

#include <string.h>
#include <sys/stat.h>

#include <stdbool.h>
#include <pthread.h>



sem_t lock;
sem_t lock2;
sem_t spaces;
sem_t items;

int pcounter = 0;
int ccounter = 0;

int *buffer;

struct pdcarg{
  int id;
  int P;
  int N;
};

struct csmarg{
  int id;
  int N;
};


int consume_item(int item){

  double i = sqrt(item);
  int tmp = (int) i;
  if(tmp == i){
      return i;
  }

  return -1;

}

void *producer(void *arg) {
        // puts("producer starting");
        struct pdcarg *param = arg;

        int i = param->id;
        int N = param->N;
        int P = param->P;

        int item = i;
        while(item < N){
        sem_wait(&spaces);
        sem_wait(&lock);
        buffer[pcounter] = item;
        // printf("%d,%d\t", pcounter, buffer[pcounter]);
        pcounter++;
        item += P;


        sem_post(&lock);
        sem_post(&items);
      }
}

void *consumer(void* arg) {
        // puts("consumer starting");-
        struct csmarg *param = arg;

        int cid = param->id;
        int N = param->N;

        int bk = 0;

        int nemp = 1;

        while(nemp){

          sem_wait(&lock2);
          if(ccounter == N){

            bk = 1;
          };
          if(bk){
            sem_post(&lock2);
            break;

          };

          sem_wait(&items);
          sem_wait(&lock);
          // printf("%d,%d\n", ccounter,N);
          if(ccounter <= N-1){
            // printf("begin ccounter: %d\n", ccounter);
            int item  = buffer[ccounter];

            ccounter++;
            int res = consume_item(item);
            if (res != -1){
              printf("cid:%d, num=%d, sqrt=%d \n", cid, item, res);
            }
            // printf("end ccounter: %d\n", ccounter);
          }else{
            nemp = 0;
          }
          sem_post(&lock);
          sem_post(&spaces);
          sem_post(&lock2);
          // printf("%d\n", nemp);
      }

}



int main (int argc, char* argv[])
{





  if (argc != 5){
   puts("ERROR: argument counts incorrect");
   exit(1);
  }


// printf("number of arg: %d\n", argc);
  int N = atoi(argv[1]);
  int B = atoi(argv[2]);
  int P = atoi(argv[3]);
  int C = atoi(argv[4]);


  sem_init(&lock, 0, 1);
  sem_init(&lock2, 0, 1);
  sem_init(&items, 0, 0);
  sem_init(&spaces, 0, B);

  struct timeval tv;
  double t1;
	double t2;


	gettimeofday(&tv, NULL);
	t1 = tv.tv_sec + tv.tv_usec/1000000.0;
  free(buffer);
  buffer = malloc(N*sizeof(int));


  pthread_t producerthread[P];

  pthread_t consumerthread[C];



  struct pdcarg producearg[P];
  struct csmarg consumearg[C];

  int i;

  for(i = 0; i < P; i++)
   {
     producearg[i].P = P;
     producearg[i].id = i;
     producearg[i].N = N;

   }


  for(i = 0; i < C; i++)
  {
    consumearg[i].N = N;
    consumearg[i].id = i;
    // printf("%d,%d\t", i,  consumearg[i].id );
  }



// puts("here");


  for(i = 0; i < P; i++)
   {
     pthread_create(&producerthread[i], NULL, producer, (void*)&producearg[i]);
   }


   for(i = 0; i < C; i++)
    {
      pthread_create(&consumerthread[i], NULL, consumer, (void*)&consumearg[i]);
    }


  for(i = 0; i < P; i++)  pthread_join(producerthread[i], NULL);
  // puts("pdc finished");
  for(i = 0; i < C; i++)  pthread_join(consumerthread[i], NULL);

  gettimeofday(&tv, NULL);
	t2 = tv.tv_sec + tv.tv_usec/1000000.0;

	printf("%.6lf seconds elapsed.\n", t2-t1);


  return 0;
}