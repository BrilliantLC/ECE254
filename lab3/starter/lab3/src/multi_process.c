#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

int sender(int n, int b, int p, int id, mqd_t qdes)
{
    pid_t child_pid;

    // create new process
    child_pid = fork();
    if (child_pid != 0)
    {
        return child_pid;
    }
    else
    {
        int num = id;
        while (num < n)
        {
            // send to mqueue
            if (mq_send(qdes, (char *)&num, sizeof(int), 0) == -1)
            {
                perror("mq_send() failed");
            };
            // such that num % p == id
            num += p;
        }

    }
    exit(0);
}

int receiver(int n, int cid, mqd_t qdes)
{
    pid_t child_pid;
    int i;

    // create new process
    child_pid = fork();
    if (child_pid != 0)
    {
        return child_pid;
    }
    else
    {
        for (int cnt = 0; cnt < n; cnt++)
        {
            // receive from mqueue
            if (mq_receive(qdes, (char *)&i, sizeof(int), 0) == -1)
            {
                perror("mq_receive() failed");
            }
            else
            {
                // calculate square root
                int sr = sqrt(i);
                if (sr * sr == i)
                {
                    printf("%d %d %d\n", cid, i, sr);
                }
            }
        }
        exit(0);
    }

}

int main(int argc, char *argv[])
{
    // input error handling
    if (argc != 5)
    {
        printf("Invalid number of arguments!\n");
        exit(1);
    }

    // read command line arguments
    int n = atoi(argv[1]);
    int b = atoi(argv[2]);
    int p = atoi(argv[3]);
    int c = atoi(argv[4]);

    struct timeval tv;
    double t1;
    double t2;

    // start timer
    gettimeofday(&tv, NULL);
    t1 = tv.tv_sec + tv.tv_usec / 1000000.0;

    // set up mailbox
    mqd_t qdes;
    const char qname[] = "/mailbox";
    mode_t mode = S_IRUSR | S_IWUSR;
    struct mq_attr attr;

    attr.mq_maxmsg = b;
    attr.mq_msgsize = sizeof(int);
    attr.mq_flags = 0; /* a blocking queue  */

    qdes = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
    if (qdes == -1)
    {
        perror("mq_open() failed");
        exit(1);
    }

    // spawning ith producer and consumer processes
    for (int i = 0; i < p; i++)
    {
        sender(n, b, p, i, qdes);
    }
    for (int i = 0; i < c; i++)
    {
        receiver(n, i, qdes);
    }

    // wait for all children to finish
    wait(NULL);


    // calculate execution time
    gettimeofday(&tv, NULL);
    t2 = tv.tv_sec + tv.tv_usec / 1000000.0;
    printf("System execution time: %.6lf seconds\n", t2 - t1);

    // close and unlink the mailbox
    if (mq_close(qdes) == -1)
    {
        perror("mq_close() failed");
        exit(2);
    }
    if (mq_unlink(qname) != 0)
    {
        perror("mq_unlink() failed");
        exit(3);
    }
    return 0;
}