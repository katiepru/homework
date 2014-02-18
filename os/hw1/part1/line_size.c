#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define ACCESS_SIZE 524288 //512 * 1024
#define ARR_SIZE 16785408

int main(int argc, char *argv[])
{
    register *a;
    register int i, dummy, iter, access;
    register long timeTaken;
    register float time_per_access;

    struct timeval before, after;

    //Page alignment
    a=sbrk(ARR_SIZE);
    a=(int *)((((int)a>>13)<<13)+8192);

    //Map it to virtual memory
    for(i = 0; i < ARR_SIZE; ++i)
    {
        dummy = a[i];
    }

    for(access = ACCESS_SIZE; access >= 2; access/=2)
    {
        gettimeofday(&before, NULL);
        for(iter = 0; iter < 10000; ++iter)
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

        printf("It took %f microseconds at jump size %d\n", time_per_access, access);
    }

    return 0;
}
