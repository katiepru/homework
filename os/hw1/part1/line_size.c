#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#define ARR_SIZE 10240

int main(int argc, char *argv[])
{
    register int *a;
    register int i, dummy, iter, access;
    register long timeTaken;
    register float time_per_access;

    struct timeval before, after;

    //Page alignment
    a=sbrk(ARR_SIZE);
    a=(int *)((((long)a>>13)<<13)+8192);

    //Map it to virtual memory
    for(i = 0; i < ARR_SIZE; ++i)
    {
        dummy = a[i];
    }

    for(access = 1; access < ARR_SIZE; access*=2)
    {
        gettimeofday(&before, NULL);
        for(iter = 0; iter < 1000000; ++iter)
        {
            for(i = 0; i < ARR_SIZE; i += access)
            {
                dummy = a[i];
            }
        }
        gettimeofday(&after, NULL);

        timeTaken = (after.tv_sec * 1000000 + after.tv_usec) -
                    (before.tv_sec*1000000 + before.tv_usec);

        time_per_access = timeTaken/(i/access);

        printf("%d %f\n", access, time_per_access);
    }

    return 0;
}
