#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

//16 ints for the registers, 1 for x, 1 for return address
//#define OFFSET 72
#define OFFSET (18*sizeof(int))

void catch()
{
    int x;
    static int count = 0;
    if(++count == 1000)
        *(&x + (OFFSET/sizeof(int))) += 8;
}

int main(int argc, char *argv[])
{
    int a, current_time;
    struct timeval before, after;

    signal(SIGFPE, catch);

    gettimeofday(&before, NULL);
    a = 1/0;
    gettimeofday(&after, NULL);
    current_time = ((after.tv_sec * 1000000) + after.tv_usec)
                 - ((before.tv_sec * 1000000) + before.tv_usec);

    printf("1000 iterations took %d microseconds total.\n", current_time);
    return 0;
}
