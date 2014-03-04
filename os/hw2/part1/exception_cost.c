#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

void catch()
{
    unsigned char x;
    static int count = 0;
    if(++count == 1000)
        *(int *)(&x + 69) += 8;
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
