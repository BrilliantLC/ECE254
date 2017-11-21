/**
 * @file:   os/lab1/mqueue/sender.c
 * @brief:  send random points to a POSIX queue. Type 'q' to terminate. 
 * @author: ece254/mte241 lab staff 
 * @date:   2012/09/20
 * NOTES: 
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>

double ta;
double tb;
double tc;
struct timeval tv;

void producer(int n, int b, int p, int id)
{
    //creating mailbox
    mqd_t qdes;
    const char qname[] = "/mailbox7"; //queue name must start with '/'. man mq_overview
    mode_t mode = S_IRUSR | S_IWUSR;
    struct mq_attr attr;

    //set msgsize to 4 and mailbox size to b
    attr.mq_msgsize = 4;
    attr.mq_maxmsg = b;

    attr.mq_flags = 0; // blocking queue

    qdes = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
    if (qdes == -1)
    {
        perror("mq_open() failed");
        exit(1);
    }

    int i;

    for (i = 0; i < n; i++)
    {
        //generate number to send

        if (i % p == id)
        {
            //send number
            //printf("sent %d\n", i);

            if (mq_send(qdes, (char *)&i, 4, 0) == -1)
            {
                perror("mq_send() failed");
            }
        }
    }

    //if (mq_close(qdes) == -1) {
    //perror("mq_close() failed\n");
    //exit(2);
    //}

    //if (mq_unlink(qname) != 0) {
    //perror("mq_unlink() failed\n");
    //exit(3);
    //}
}

int main(int argc, char *argv[])
{
    //call on producer method
    producer(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    //p, n, b, id

    return 0;
}