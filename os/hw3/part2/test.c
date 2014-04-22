#include "mypthread.h"

void *func(void *args)
{
    while(1);
    printf("I am a function\n");
    mypthread_yield();
    printf("Done yieling bitch\n");
    mypthread_exit(NULL);
    return NULL;
}

void *func2(void *args)
{
    while(1)
    {
        printf("func2\n");
        mypthread_yield();
    }
    printf("I'm great\n");
    mypthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[])
{
    mypthread_t thread1, thread2;
    printf("Creatinf first thread\n");
    mypthread_create(&thread1, func, NULL);
    printf("Creating second thread\n");
    mypthread_create(&thread2, func2, NULL);

    printf("Joining %d\n", thread1);
    mypthread_join(thread1, NULL);

    printf("Joining %d\n", thread2);
    mypthread_join(thread2, NULL);

    printf("In main\n");

    return 0;
}
