#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <signal.h>
#include <math.h>

#define _XOPEN_SOURCE 600

struct timeval tv;

int main(int argc, char *argv[])
{
    //open existing mailbox
    int pt;
    mqd_t qdes;
    const char qname[] = "/mailbox7";
    mode_t mode = S_IRUSR | S_IWUSR;
    struct mq_attr attr;

    int n = atoi(argv[1]);  //n
    int b = atoi(argv[2]);  //b
    int c = atoi(argv[3]);  //c
    int id = atoi(argv[4]); //id

    attr.mq_maxmsg = b;
    attr.mq_msgsize = 32;
    attr.mq_flags = 0;

    double ta;
    double tb;

    //delay so receiver doesn't consume number before it is produced
    sleep(1);

    qdes = mq_open(qname, O_RDONLY, mode, &attr);
    if (qdes == -1)
    {
        perror("mq_open()");
        exit(1);
    }

    int i = 0;
    while (i < n)
    {

        if (i % c == id)
        {
            //attempt to receive number
            if (mq_receive(qdes, (char *)&pt, 32, 0) == -1)
            {
                perror("mq_timedreceive() failed\n");
            }
            else
            {
                int sq = sqrt(pt);
                if (sq * sq == pt)
                {
                    printf("c%d %d %d\n", id, pt, sq);
                }
            }
        }
        i++;
    }

    //transferring ta
    //ta = atof(argv[5]);

    //printf("System execution time: %d seconds\n", tb-ta);

    //if (mq_close(qdes) == -1) {
    //perror("mq_close() failed");
    //exit(2);
    //}

    return 0;
}