#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#define ARR_SIZE 2097152 //2 megs

int main(int argc, char *argv[])
{
    register int *a;
    register int i, dummy, iter, access, arr_size_partial;
    register long timeTaken;
    register float time_per_access;

    struct timeval before, after;

    if(argc != 2)
    {
        fprintf(stderr, "Not enough args: include a line size\n");
        exit(1);
    }

    access = atoi(argv[1]);

    a=sbrk(ARR_SIZE);
    a=(int *)((((long)a>>13)<<13)+8192);

    //Map it to virtual memory
    for(i = 0; i < ARR_SIZE; ++i)
    {
        dummy = a[i];
    }

    for(arr_size_partial = access; arr_size_partial <= ARR_SIZE; arr_size_partial *= 2)
    {
        gettimeofday(&before, NULL);
        for(iter = 0; iter < 1000000; ++iter)
        {
            for(i = 0; i < arr_size_partial; ++i)
            {
                dummy = a[i];
            }
        }
        gettimeofday(&after, NULL);

        timeTaken = (after.tv_sec * 1000000 + after.tv_usec) -
                    (before.tv_sec*1000000 + before.tv_usec);

        time_per_access = timeTaken/i;

        printf("It took %f microseconds at arr size %d\n", time_per_access, arr_size_partial);
    }
}
