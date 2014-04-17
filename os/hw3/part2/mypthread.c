#include "mypthread.h"

void mypthread_create(mypthread_t *thread, thread_func f, void *args)
{
    static int tid = 0;
    static char init = 0;

    thread = malloc(sizeof(mypthread_t));
    thread->tid = tid++;
    thread->status = NEW;

    if(!init)
    {
        //This is our first thread being created
        //Do some setup
        
    }

}

//Internal functions

void scheduler()
{
}
