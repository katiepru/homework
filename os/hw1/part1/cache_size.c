#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#define ARR_SIZE (1024 * 4096) //4 megs

int main(int argc, char *argv[])
{
    register volatile int *a;
    register int i, dummy, iter, access, arr_size_partial;
    register long timeTaken;
    register float time_per_access;

    struct timeval before, after;

    if(argc != 3)
    {
        fprintf(stderr, "Not enough args: include a line size in Bs and walk size in KBs\n");
        exit(1);
    }

    access = atoi(argv[1]);
    arr_size_partial = atoi(argv[2])*1024;

    a=sbrk(ARR_SIZE*sizeof(int)+8192);
    a=(int *)((((int)a>>13)<<13)+8192);

    //Map it to virtual memory
    for(i = 0; i < ARR_SIZE; ++i)
    {
        dummy = a[i];
    }

    gettimeofday(&before, NULL);
    for(iter = 0; iter < 1000000; ++iter)
    {
        for(i = 0; i < arr_size_partial; i += access)
        {
            dummy = a[i];
        }
    }
    gettimeofday(&after, NULL);

    timeTaken = (after.tv_sec * 1000000 + after.tv_usec) -
                (before.tv_sec*1000000 + before.tv_usec);

    time_per_access = timeTaken/(arr_size_partial/access);

    printf("%d %f\n", arr_size_partial/1024, time_per_access);

    return 0;
}
