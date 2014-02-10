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

    char *c;

    int pid;
    int mbs;
    int i;

    //Check arguments
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./part2 <MBs>\n");
        return 2;
    }

    mbs = atoi(argv[1]);

    //Malloc a chunk to change program size
    c = malloc(mbs * 1048576);

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

        //Compute time that operation took in microseconds
        current_time = ((after.tv_sec * 1000000) + after.tv_usec)
                     - ((before.tv_sec * 1000000) + before.tv_usec);

        //Detect errors in time struct
        if(current_time <= 0)
        {
            fprintf(stderr, "The %dth fork took negative time\n", i);
            return 1;
        }

        //Add to total time
        total_time += current_time;

    }

    //Compute average time from total time
    avg_time = ((float) total_time)/10000;

    printf("Each fork took %f microseconds on average.\n", avg_time);


    return 0;
}
