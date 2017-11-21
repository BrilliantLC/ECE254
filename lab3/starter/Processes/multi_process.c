#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

int spawn(char *program, char **arg_list)
{
    pid_t child_pid;

    /* Duplicate this process.  */
    child_pid = fork();
    if (child_pid != 0)
        /* This is the parent process.  */
        return child_pid;
    else
    {
        /* Now execute PROGRAM, searching for it in the path.  */
        execvp(program, arg_list);
        /* The execvp function returns only if an error occurs.  */
        fprintf(stderr, "an error occurred in execvp\n");
        abort();
    }
}

int sender(int argc, char *argv[])
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
    attr.mq_msgsize = sizeof(int);
    attr.mq_flags = 0; /* a blocking queue  */
    n = atoi(argv[1]);
    b = atoi(argv[2]);
    p = atoi(argv[3]);
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

int receiver(int argc, char *argv[])
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
                int sq = sqrt(pt);
                if (sq * sq == pt)
                {
                    printf("c%d %d %d\n", id, pt, sq);
                }
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Invalid number of arguments!");
        exit(1);
    }

    int N = atoi(argv[1]);
    int B = atoi(argv[2]);
    int P = atoi(argv[3]);
    int C = atoi(argv[4]);

    /* from sample code "time.c" */
    struct timeval tv;
    double t1;
    double t2;

    gettimeofday(&tv, NULL);
    t1 = tv.tv_sec + tv.tv_usec / 1000000.0;

    // spawning producer and consumer processes
    for (int i = 0; i < P; i++)
    {
        spawnp(i, B, P, N);
    }
    for (int i = 0; i < C; i++)
    {
        spawnc(i, B, N);
    }

    pid_t pid;

    // wait till all the children process result has been collected
    while (pid = waitpid(-1, NULL, 0))
    {
        if (errno == ECHILD)
            break;
    }

    gettimeofday(&tv, NULL);
    t2 = tv.tv_sec + tv.tv_usec / 1000000.0;

    printf("%.6lf seconds elapsed.\n", t2 - t1);
    return 0;
}