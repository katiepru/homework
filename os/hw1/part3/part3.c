#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    struct timeval before, after;

    char *arr;
    float avg_time;
    int pid, i, arr_size, current_time, mbs, total_time=0;
    int fdp[2], fdc[2];

    //Check arguments
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./part3 <MBs>\n");
        return 2;
    }

    mbs = atoi(argv[1]);

    arr = calloc(mbs * 1048576, sizeof(char));
    arr_size = mbs * 1048576, sizeof(char);

    pipe(fdp);
    pipe(fdc);

    pid = fork();

    //In the child
    if(pid == 0)
    {
        close(fdc[0]);
        close(fdp[1]);

        for(i = 0; i < 10000; ++i)
        {
            read(fdp[0], &arr, arr_size);
            write(fdc[1], &arr, arr_size);
        }
        exit(0);
    }

    //In the parent
    else
    {
        close(fdc[1]);
        close(fdp[0]);

        for(i = 0; i < 10000; ++i)
        {
            gettimeofday(&before, NULL);
            write(fdp[1], &arr, arr_size);
            read(fdc[0], &arr, arr_size);
            gettimeofday(&after, NULL);

            current_time = ((after.tv_sec * 1000000) + after.tv_usec)
                         - ((before.tv_sec * 1000000) + before.tv_usec);
            total_time += current_time;
        }

        avg_time = ((float) total_time)/10000;
        printf("%f\n", avg_time);

        waitpid(pid, NULL, 0);
        exit(0);
    }
}
