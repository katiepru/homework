#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct timeval before;
    struct timeval after;

    long total_time = 0;
    long current_time;

    float avg_time;

    int pid;
    int i;

    //TODO: Implement different program sizes

    for (i = 0; i < 10000; ++i)
    {
        gettimeofday(&before, NULL);

        pid = fork();

        gettimeofday(&after, NULL);

        //In the child - die
        if(pid == 0)
        {
            return 0;
        }

        current_time = ((after.tv_sec * 1000000) + after.tv_usec)
                     - ((before.tv_sec * 1000000) + before.tv_usec);

        if(current_time <= 0)
        {
            fprintf(stderr, "The %dth fork took negative time\n", i);
            return 1;
        }

        total_time += current_time;

    }

    avg_time = ((float) total_time)/10000;

    printf("Each fork took %f microseconds on average.\n", avg_time);


    return 0;
}
