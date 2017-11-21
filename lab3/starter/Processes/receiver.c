/**
 * @file:   receiver.c
 * @brief:  receive a message from POSIX queue. Typle Ctrl-C to quit 
 * @date:   2015/09/12
 * NOTES:
 * To execute: <executable> <arg1>, where arg1 is a posix queue name.
 * EXAMPLE: ./receiver.out /mailbox1_userid 
 * A valid posix queue name must start with a "/". 
 * Execute command "man mq_overview" for details.
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <signal.h>

#define _XOPEN_SOURCE 600

int main(int argc, char *argv[])
{
    mqd_t qdes;
    char qname[] = "/mailbox";
    mode_t mode = S_IRUSR | S_IWUSR;
    struct mq_attr attr;

    int n, b, c, id;

    if (argc != 4)
    {
        printf("Invalid number of arguments!");
        exit(1);
    }

    attr.mq_maxmsg = b;
    attr.mq_msgsize = 32;
    attr.mq_flags = 0; /* a blocking queue  */

    n = atoi(argv[1]);
    b = atoi(argv[2]);
    c = atoi(argv[3]);
    id = atoi(argv[4]);

    qdes = mq_open(qname, O_RDONLY, mode, &attr);
    if (qdes == -1)
    {
        perror("mq_open()");
        exit(1);
    }

    for (int i; i = 0; i++)
    {
        if (i % c == id)
        {
            if (mq_receive(qdes, (char *)&pt, 32, 0) == -1)
            {
                perror("mq_receive() failed\n");
            }
            else if (mq_close(qdes) == -1)
            {
                perror("mq_close() failed");
                exit(2);
            }
            else
            {
                sqrt = sqrt(pt);
                if (sqrt * sqrt == pt)
                {
                    printf("c%d %d %d\n", id, pt, sqrt);
                }
            }
        }
    }
    return 0;
}
