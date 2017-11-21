// ECE254 lab 3 code by group 22
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>

// global variables
int N, B, P, C;

void producer() {
    while(1){
        produce_item();
        sem_wait(spaces);
        sem_wait(lock);
        add_item();  // add to buffer
        sem_post(lock);
        sem_post(items);
    }
}

void consumer() {
    while(1){
        sem_wait(items);
        sem_wait(lock);
        consume_item();  // remove from buffer
        sem_post(lock);
        sem_post(spaces);
    }
}

void produce_item()
{
    int i = 
    return;
}
void add_item(){
    return;
}

void consume_item(){
    return;
}

void init(){
    semaphore_create(items, N, 0);  // number of integers produced, 0 to N
    semaphore_create(spaces, B, B);  // empty space in buffer, B to 0
    semaphore_create(lock, 1, 1);  // mutex default at 1, unlocked
    return;
}

/* Spawn a child process running a new program.  PROGRAM is the name
   of the program to run; the path will be searched for this program.
   ARG_LIST is a NULL-terminated list of character strings to be
   passed as the program's argument list.  Returns the process id of
   the spawned process.  */

int spawn (char* program, char** arg_list)
{
  pid_t child_pid;

  /* Duplicate this process.  */
  child_pid = fork ();
  if (child_pid != 0)
    /* This is the parent process.  */
    return child_pid;
  else {
    /* Now execute PROGRAM, searching for it in the path.  */
    execvp (program, arg_list);
    /* The execvp function returns only if an error occurs.  */
    fprintf (stderr, "an error occurred in execvp\n");
    abort ();
  }
}

int main (int argc, char *argv[])
{
  /* The argument list to pass to the "ls" command.  */
  char* arg_list[] = {
    "ls",     /* argv[0], the name of the program.  */
    "-l", 
    "/",
    NULL      /* The argument list must end with a NULL.  */
  };

  /* Spawn a child process running the "ls" command.  Ignore the
     returned child process id.  */
  spawn ("ls", arg_list); 

  printf ("done with main program\n");

  return 0;
}

int getTime(void)
{
	struct timeval tv;
	double t1;
	double t2;

	gettimeofday(&tv, NULL);
	t1 = tv.tv_sec + tv.tv_usec/1000000.0;

	sleep(1);
	usleep(50);

	gettimeofday(&tv, NULL);
	t2 = tv.tv_sec + tv.tv_usec/1000000.0;

	printf("System excution time: %.6lf.\n", t2-t1);

	return 0;
}

// from sender.c
// sends an integer to a pqueue
// ie. add to buffer
int add_item(int argc, char *argv[])
{
	mqd_t qdes;
	char  quit = '\0';

	/* queue name must start with '/'. man mq_overview */
	//char  qname[] = "/mailbox1_yqhuang";	
	char *qname = NULL;

	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;

	if ( argc !=2 ) {
		printf("Usage: %s <qname>\n", argv[0]);
		printf("The qname must start with a \"/\". \n");
		printf("An example qname: /mailbox1_userid\n");
		exit(1);
	}
	
	qname = argv[1];

	attr.mq_maxmsg  = QUEUE_SIZE;
	attr.mq_msgsize = sizeof(struct point);
	attr.mq_flags   = 0;		/* a blocking queue  */

	qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
	if (qdes == -1 ) {
		perror("mq_open() failed");
		exit(1);
	}

	srand(time(0));

	printf("Press a key to send a random point.\n");
	getchar();
	do {
		struct point pt;

		set_position(rand() % 80, rand() % 24, &pt); 
		if (mq_send(qdes, (char *)&pt, sizeof(struct point), 0) == -1) {
			perror("mq_send() failed");
		}
		printf("Sending a random point at (%d, %d)...\n", \
		       get_x_coord(pt), get_y_coord(pt));
		quit = getchar();
	} while (quit != 'q');


	if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}

	if (mq_unlink(qname) != 0) {
		perror("mq_unlink() failed");
		exit(3);
	}

	return 0;
}