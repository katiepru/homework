#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct timeval before, after;

    long total_time = 0;
    int current_time;

    float avg_time;

    char *c;

    int pid, mbs, i, ret;

    int fd[2];

    //Check arguments
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./part2 <MBs>\n");
        return 2;
    }

    mbs = atoi(argv[1]);

    //Malloc a chunk to change program size
    c = calloc(mbs * 1048576, sizeof(char));

    for (i = 0; i < 10000; ++i)
    {
        pipe(fd);

        gettimeofday(&before, NULL);
        pid = fork();
        gettimeofday(&after, NULL);

        if(pid != 0)
        {
            waitpid(pid, NULL, 0);
            close(fd[1]);
            read(fd[0], &ret, sizeof(ret));
            total_time += ret;
            close(fd[0]);
        }

        else
        {
            close(fd[0]);

            //Compute time that operation took in microseconds
            current_time = ((after.tv_sec * 1000000) + after.tv_usec)
                         - ((before.tv_sec * 1000000) + before.tv_usec);

            write(fd[1], &current_time, sizeof(current_time));

            free(c);
            close(fd[1]);
            exit(0);
        }

    }

    //Compute average time from total time
    avg_time = ((float) total_time)/10000;

    printf("%f\n", avg_time);

    free(c);

    return 0;
}
