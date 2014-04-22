#include "mypthread.h"

_mypthread_t **threads;
mypthread_t running_thread;
Queue *run_queue;
int curr_tid;
char in_lib = 0;
struct itimerval timer;


//Thread user-facing functions

void mypthread_create(mypthread_t *threadID, thread_func f, void *args)
{
    static int tid = 1; //First thread is for main
    static char init = 0;
    _mypthread_t *main_thread, *thread;

    in_lib = 1;

    if(!init)
    {
        init = 1;
        threads = calloc(512, sizeof(_mypthread_t *));

        //Init run queue
        run_queue = queue_init();

        //Register timer handler
        signal(SIGVTALRM, timer_handler);

        //Init timer
        timer.it_value.tv_sec = 0 ;
        timer.it_value.tv_usec = 10000;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 10000 ;
        setitimer(ITIMER_VIRTUAL, &timer, NULL);

        //Create main thread
        main_thread = thread_init(0);
        running_thread = 0;
        main_thread->status = RUNNING;

    }


    *threadID = tid++;
    thread = thread_init(*threadID);

    makecontext(&(thread->context), f, 1, args);
    push(run_queue, threadnode_init(*threadID));
    scheduler();
}

void mypthread_exit(void *ret)
{
    in_lib = 1;
    threads[running_thread]->retval = ret;
    threads[running_thread]->status = KILLED;
    scheduler();
}

void mypthread_join(mypthread_t thread, void **ret)
{
    in_lib = 1;
    if(threads[thread] == NULL) return;
    while(threads[thread]->status != KILLED)
    {
        mypthread_yield();
    }

    ret = &(threads[thread]->retval);
    in_lib = 0;
}

void mypthread_yield()
{
    scheduler();
}

//Mutex user-facing functions

//Initialize a mutex
int mypthread_mutex_init(mypthread_mutex_t *mutex, char *garbage)
{
    *mutex = malloc(sizeof(_mypthread_mutex_t));
    if(*mutex == NULL) return 1;
    (*mutex)->locked = 0;
    (*mutex)->tid_locked_by = -1;
    return 0;
}

//Destroy a mutex
int mypthread_mutex_destroy(mypthread_mutex_t *mutex)
{
    if((*mutex)->locked) return 1;
    free(*mutex);
    return 0;
}

//Unlock a mutex
int mypthread_mutex_unlock(mypthread_mutex_t *mutex)
{
    if(!(*mutex)->locked) return 1;
    if((*mutex)->tid_locked_by != running_thread) return 2;
    (*mutex)->locked = 0;
    (*mutex)->tid_locked_by = -1;
    return 0;
}

//Lock a mutex
int mypthread_mutex_lock(mypthread_mutex_t *mutex)
{
    while((*mutex)->locked)
    {
        mypthread_yield();
    }
    (*mutex)->locked = 1;
    (*mutex)->tid_locked_by = running_thread;
    return 0;
}

//Try to lock a mutex
int mypthread_mutex_trylock(mypthread_mutex_t *mutex)
{
    if((*mutex)->locked) return 1;
    (*mutex)->locked = 1;
    (*mutex)->tid_locked_by = running_thread;
    return 0;
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
    in_lib = 0;
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

void timer_handler(int signum)
{
    printf("handling\n");
    if(in_lib)
    {
        printf("doing nothing\n");
        return; //Do nothing if we are already in the library
    }
    in_lib = 1;
    scheduler();
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
