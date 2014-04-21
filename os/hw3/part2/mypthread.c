#include "mypthread.h"

_mypthread_t **threads;
mypthread_t running_thread;
Queue *run_queue;
int curr_tid;

void mypthread_create(mypthread_t *threadID, thread_func f, void *args)
{
    static int tid = 1; //First thread is for main
    static char init = 0;
    _mypthread_t *main_thread, *thread;

    if(!init)
    {
        init = 1;
        threads = calloc(512, sizeof(_mypthread_t *));

        //Init run queue
        run_queue = queue_init();

        //Create main thread
        main_thread = thread_init(0);
        running_thread = 0;

    }


    *threadID = tid++;
    thread = thread_init(*threadID);

    makecontext(&(thread->context), f, 1, args);
    push(run_queue, threadnode_init(*threadID));
    scheduler();
}

void mypthread_exit(void *ret)
{
    threads[running_thread]->retval = ret;
    threads[running_thread]->status = KILLED;
    scheduler();
}

void mypthread_join(mypthread_t thread, void **ret)
{
    while(threads[thread]->status != KILLED)
    {
        mypthread_yield();
    }

    ret = &(threads[thread]->retval);
}

void mypthread_yield()
{
    scheduler();
}

//Internal functions

void scheduler()
{
    mypthread_t next_runner;
    mypthread_t curr = running_thread;
    ThreadNode *enqueued, *dequeued;

    //Enter current thread into run queue
    if(threads[curr]->status != KILLED)
    {
        threads[curr]->status = WAITING;
        enqueued = threadnode_init(curr);
        enqueue(run_queue, enqueued);
    }

    //Get next thread to run
    do
    {
        dequeued = dequeue(run_queue);
        if(dequeued == NULL)
        {
            printf("Out of things to run\n");
            return;
        }
        next_runner = dequeued->thread;
    } while(threads[next_runner]->status != WAITING);

    running_thread = next_runner;
    threads[running_thread]->status = RUNNING;
    swapcontext(&(threads[curr]->context), &(threads[next_runner]->context));
}

_mypthread_t *thread_init(mypthread_t threadID)
{
    char *stack;
    _mypthread_t *ret = malloc(sizeof(_mypthread_t));

    threads[threadID] = ret;
    ret->tid = threadID;
    ret->retval = NULL;
    ret->status = WAITING;
    if(getcontext(&(ret->context)) == -1)
    {
        fprintf(stderr, "getcontext failed\n");
        exit(5);
    }
    stack = malloc(STACK_SIZE*sizeof(char));
    ret->context.uc_stack.ss_sp = stack;
    ret->context.uc_stack.ss_size = STACK_SIZE;
    //FIXME: Successor

    return ret;
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

//Push to front of queue - ignore all queue logic
void push(Queue *q, ThreadNode *item)
{
    if(q->back == NULL)
    {
        //We are empty
        q->back = item;
    }
    item->next = q->front;
    q->front = item;
    q->length++;
}

//Enqueue an item into q
void enqueue(Queue *q, ThreadNode *item)
{
    if(q->front == NULL)
    {
        //We are empty
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
    if(q->front == NULL)
    {
        return NULL;
    }
    q->front = q->front->next;
    ret->next = NULL;
    return ret;
}

//Init a ThreadNode
ThreadNode *threadnode_init(mypthread_t thread)
{
    ThreadNode *t = malloc(sizeof(ThreadNode));
    t->thread = thread;
    t->next = NULL;
    return t;
}
