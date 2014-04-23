#include "mypthread.h"

mypthread_mutex_t mutex;

void *func(void *args)
{
    printf("Locking mutex in func\n");
    mypthread_mutex_lock(&mutex);
    printf("Yielding from func\n");
    mypthread_yield();
    printf("unlocking mutex in func\n");
    mypthread_mutex_unlock(&mutex);
    mypthread_exit(NULL);
    return NULL;
}

void *func2(void *args)
{
    printf("locking mutex in func2\n");
    mypthread_mutex_lock(&mutex);
    printf("Got it, now unlocking in func2\n");
    mypthread_mutex_unlock(&mutex);
    mypthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[])
{
    mypthread_mutex_init(&mutex, NULL);
    mypthread_t thread1, thread2;
    printf("Creatinf first thread\n");
    mypthread_create(&thread1, NULL, func, NULL);
    printf("Creating second thread\n");
    mypthread_create(&thread2, NULL, func2, NULL);

    printf("Joining %d\n", thread1);
    mypthread_join(thread1, NULL);

    printf("Joining %d\n", thread2);
    mypthread_join(thread2, NULL);

    mypthread_mutex_destroy(&mutex);

    printf("In main\n");

    return 0;
}
