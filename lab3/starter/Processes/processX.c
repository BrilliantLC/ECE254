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




// compute sqareroot

int consume_item(int item){

  double i = sqrt(item);
  int tmp = (int) i;
  if(tmp == i){
      return i;
  }

  return -1;

}

// send item to mq
 void producer(int item, mqd_t qdes) {

		     if(mq_send(qdes, (char *)&item, sizeof(int), 0) == -1){
           perror("mq_send() failed");
         }; // add to buffer
 }

// consume item from mq
 int consumer(int cid, mqd_t qdes, struct timespec ts, int N) {


         int item;
        if( mq_timedreceive(qdes, (char *) &item, sizeof(int), 0, &ts)== -1){
             // if there is an error, most likely there is no item in the mq
             // so return to collect result
             return -1;
         }else{
           int res = consume_item(item);
           // calculate square root
           if (res != -1){
             printf("cid:%d, num=%d, sqrt=%d \n", cid, item, res);
           }
        }

        return 0;

 }

// spawn producer process
int spawnp (int i, int B, int P, int N)
{
  pid_t child_pid;

  /* Duplicate this process.  */
  child_pid = fork ();
  if (child_pid != 0)
    /* This is the parent process.  */
    return child_pid;
  else {
    int item = i;

    // configuration from mq sample code
    mqd_t qdes;
    char *qname = NULL;
    mode_t mode = S_IRUSR | S_IWUSR;
    struct mq_attr attr;
    qname = "/mbx_h395li";
    attr.mq_maxmsg  = 20;
    attr.mq_msgsize = sizeof(int);
    attr.mq_flags   = 0;		/* a blocking queue  */

    qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
    if (qdes == -1 ) {
      perror("mq_open() failed");

    }
    while(item < N){

      producer(item,qdes);

      item += P;
    }


    if (mq_close(qdes) == -1) {
      // perror("mq_close() failed");
      exit(2);
    }

    if (mq_unlink(qname) != 0) {
      // perror("mq_unlink() failed");
      exit(3);
    }
//
    abort ();
  }
}

int spawnc (int i, int B, int N)
{


  pid_t child_pid;

  /* Duplicate this process.  */
  child_pid = fork ();
  if (child_pid != 0)
    /* This is the parent process.  */
    return child_pid;
  else {
    // puts("consumer process creating");
    mqd_t qdes;
  	char  *qname = NULL;
  	mode_t mode = S_IRUSR | S_IWUSR;
  	struct mq_attr attr;

  	    qname = "/mbx_h395li";

  	attr.mq_maxmsg  = B;
    attr.mq_msgsize = sizeof(int);
  	attr.mq_flags   = 0;	/* a blocking queue  */

  	qdes  = mq_open(qname, O_RDONLY, mode, &attr);
    if (qdes == -1 ) {
  		// perror("mq_open() failed");
  		exit(1);
  	}
  	if (qdes == -1 ) {
  		// perror("mq_open()");
  		exit(1);
  	}
  	if (qdes == -1 ) {
  		// perror("mq_open()");
  		exit(1);
  	}
    int res = 0;
    while(res == 0){
      struct timespec ts = {time(0) + 1, 0};
      res = consumer(i, qdes, ts, N);
    }


    abort ();
  }
}

int main (int argc, char* argv[])
{


  if (argc != 5){
   puts("ERROR: argument counts incorrect");
   exit(1);
  }

  int N = atoi(argv[1]);
  int B = atoi(argv[2]);
  int P = atoi(argv[3]);
  int C = atoi(argv[4]);

  struct timeval tv;
  double t1;
	double t2;

	gettimeofday(&tv, NULL);
	t1 = tv.tv_sec + tv.tv_usec/1000000.0;


  for(int i = 0; i<P; i++){
    spawnp(i, B, P, N);
  }
  for(int i = 0; i<C; i++){
    spawnc(i, B, N);
  }

  pid_t pid;

  // wait till all the children process result has been collected
  while (pid = waitpid (-1, NULL, 0)){
      if (errno == ECHILD)
          break;
  }



  gettimeofday(&tv, NULL);
	t2 = tv.tv_sec + tv.tv_usec/1000000.0;

	printf("%.6lf seconds elapsed.\n", t2-t1);


  return 0;
}