#include "mypthread.h"

mypthread_t **threads;

void mypthread_create(mypthread_t *thread, thread_func f, void *args)
{
    static int tid = 2; //First thread is for main, second for sched
    static char init = 0;
    mypthread_t *main_thread, *sched_thread;
    char *main_stack, *sched_stack, *thread_stack;

    if(!init)
    {
        init = 1;
        threads = calloc(512, sizeof(mypthread_t *));

        //Create main thread
        main_thread = malloc(sizeof(mypthread_t));
        //FIXME: Implement this
        tid++;

        //Create scheduler thread
        sched_thread = malloc(sizeof(mypthread_t));
        sched_thread->tid = tid++;
        sched_thread->status = WAITING;
        if(getcontext(&(sched_thread->context)) == -1)
        {
            fprintf(stderr, "getcontext failed\n");
            exit(5);
        }
        sched_stack = malloc(STACK_SIZE*sizeof(char));
        sched_thread->context.uc_stack.ss_sp = sched_stack;
        sched_thread->context.uc_stack.ss_size = STACK_SIZE;
        //FIXME: Successor
        makecontext(&(sched_thread->context), scheduler, 0);

    }

    thread = malloc(sizeof(mypthread_t));
    thread->tid = tid++;
    thread->status = NEW;
    if(getcontext(&(thread->context)) == -1)
    {
        fprintf(stderr, "getcontext failed\n");
        exit(5);
    }
    thread_stack = malloc(STACK_SIZE*sizeof(char));
    thread->context.uc_stack.ss_sp = thread_stack;
    thread->context.uc_stack.ss_size = STACK_SIZE;
    //FIXME: Successor
    makecontext(&(thread->context), f, 1, args);

}

//Internal functions

void scheduler()
{
}
