#include "mypthread.h"

mypthread_t **threads;
mypthread_t *running_thread;
Queue *run_queue;
int curr_tid;

void mypthread_create(mypthread_t *thread, thread_func f, void *args)
{
    static int tid = 1; //First thread is for main
    static char init = 0;
    mypthread_t *main_thread;
    char *main_thread_stack, *thread_stack;

    if(!init)
    {
        init = 1;
        threads = calloc(512, sizeof(mypthread_t *));

        //Create main thread
        main_thread = malloc(sizeof(mypthread_t));
        threads[0] = main_thread;
        main_thread->tid = 0;
        main_thread->status = RUNNING;
        running_thread = main_thread;
        if(getcontext(&(main_thread->context)) == -1)
        {
            fprintf(stderr, "getcontext failed\n");
            exit(5);
        }
        main_thread_stack = malloc(STACK_SIZE*sizeof(char));
        main_thread->context.uc_stack.ss_sp = main_thread_stack;
        main_thread->context.uc_stack.ss_size = STACK_SIZE;
        //FIXME: Successor
    }

    thread = malloc(sizeof(mypthread_t));
    thread->tid = tid++;
    thread->status = WAITING;
    running_thread = thread;
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
    f(args);
    return;
}

void mypthread_yield()
{
    scheduler();
}

//Internal functions

void scheduler()
{
    mypthread_t *next_runner;
    mypthread_t *curr = running_thread;

    //Check if we have something to run
    if(run_queue->length = 0) return;

    //Get next thread to run
    do
    {
        next_runner = dequeue(run_queue)->thread;
    } while(next_runner->status != WAITING);

    //Enter current thread into run queue
    curr->status = WAITING;
    enqueue(run_queue, curr);

    running_thread = next_runner;
    swapcontext(&(curr->context), &(next_runner->context));

}


//Queue functions

//Init a queue
Queue *queue_init()
{
    Queue *q = malloc(sizeof(Queue));
    q->front = NULL;
    q->back = NULL;
    q->length = 0;
    return q;
}

//Destroy a queue
void queue_destroy(Queue *q)
{
    ThreadNode *curr;
    ThreadNode *next;

    for(curr = q->front; curr != NULL; curr = next)
    {
        next = curr->next;
        free(curr);
    }

    free(q);
}

//Enqueue an item into q
void enqueue(Queue *q, ThreadNode *item)
{
    if(q->front == NULL)
    {
        q->front = item;
    } else
    {
        q->back->next = item;
    }
    q->back = item;
    q->length++;
}

//Dequeue an item from q
ThreadNode *dequeue(Queue *q)
{
    ThreadNode *ret = q->front;
    q->front = q->front->next;
    ret->next = NULL;
    return ret;
}
