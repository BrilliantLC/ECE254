/**
 * @file:   sender.c
 * @brief:  send random points to a POSIX queue. Type 'q' to terminate. 
 * @date:   2014/06/25
 * NOTES:
 * To execute: <executable> <arg1>, where arg1 is a posix queue name.
 * EXAMPLE: ./sender.out /mailbox1_userid 
 * A valid posix queue name must start with a "/". 
 * Execute command "man mq_overview" for details.
 * Check /dev/mqueue to clean up the queue if sender fails to do so.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include "common.h"
#include "point.h"

int main(int argc, char *argv[])
{
    // create mailbox
    mqd_t qdes;
    char qname[] = "/mailbox";
    mode_t mode = S_IRUSR | S_IWUSR;
    struct mq_attr attr;

    // variables
    int n, b, p;
    int id;

    // expecting 4 arguments
    if (argc != 4)
    {
        printf("Invalid number of arguments!");
        exit(1);
    }

    attr.mq_maxmsg = b;
    attr.mq_msgsize = 4;
    attr.mq_flags = 0; /* a blocking queue  */
    n = atoi(argv[1]);
    b = atoi(argv[2]);
    c = atoi(argv[3]);
    id = atoi(argv[4]);

    qdes = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
    if (qdes == -1)
    {
        perror("mq_open() failed");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        if (i % p == id)
        {
            if (mq_send(qdes, (char *)&i, attr.mq_msgsize, 0) == -1)
            {
                perror("mq_send() failed");
            }
            else if (mq_close(qdes) == -1)
            {
                perror("mq_close() failed");
                exit(2);
            }
            else if (mq_unlink(qname) != 0)
            {
                perror("mq_unlink() failed");
                exit(3);
            }
        }
    }
    return 0;
}
