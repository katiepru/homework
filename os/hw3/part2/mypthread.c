#include "mypthread.h"

#define DEFAULT_PRIORITY 25

_mypthread_t **threads;
mypthread_t running_thread;
Queue *run_queue;
int curr_tid;
char in_lib = 0;
struct itimerval timer;
int thread_len = 512;


//Thread user-facing functions

void mypthread_create(mypthread_t *threadID, char *garabage, thread_func f, void *args)
{
    static char init = 0;
    static int tid = 1;
    _mypthread_t *main_thread, *thread;

    in_lib = 1;

    if(!init)
    {
        init = 1;
        threads = calloc(thread_len, sizeof(_mypthread_t *));

        atexit(cleanup);

        //Init run queue
        run_queue = queue_init(); //Register timer handler
        signal(SIGVTALRM, timer_handler);

        //Init timer
        timer.it_value.tv_sec = 0 ;
        timer.it_value.tv_usec = 100;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 100;
        setitimer(ITIMER_VIRTUAL, &timer, NULL);

        //Create main thread
        main_thread = thread_init(0);
        running_thread = 0;
        main_thread->status = RUNNING;

    }


    //Find first open slot
    // for(i = 0; i < thread_len; ++i)
    // {
    //     if(threads[i] == NULL)
    //     {
    //         *threadID = i;
    //         break;
    //     }
    //     if(i == thread_len - 1)
    //     {
    //         //Did not find one
    //         thread_len *= 2;
    //         threads = realloc(threads, thread_len);
    //         *threadID = i+1;
    //     }
    // }
    *threadID = tid++;

    thread = thread_init(*threadID);

    makecontext(&(thread->context), (void *)f, 1, args);
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

    if(ret != NULL)
        *ret = threads[thread]->retval;
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
    in_lib = 1;
    *mutex = malloc(sizeof(_mypthread_mutex_t));
    if(*mutex == NULL) return 1;
    (*mutex)->locked = 0;
    (*mutex)->tid_locked_by = -1;
    (*mutex)->waiting_threads = queue_init();
    in_lib = 0;
    return 0;
}

//Destroy a mutex
int mypthread_mutex_destroy(mypthread_mutex_t *mutex)
{
    in_lib = 1;
    if((*mutex)->locked) return 1;
    queue_destroy((*mutex)->waiting_threads);
    free(*mutex);
    in_lib = 0;
    return 0;
}

//Unlock a mutex
int mypthread_mutex_unlock(mypthread_mutex_t *mutex)
{
    ThreadNode *next_runner;

    in_lib = 1;
    if(!(*mutex)->locked) return 1;
    if((*mutex)->tid_locked_by != running_thread) return 2;
    (*mutex)->locked = 0;
    (*mutex)->tid_locked_by = -1;

    next_runner = dequeue((*mutex)->waiting_threads);
    if((next_runner != NULL))
    {
        threads[next_runner->thread]->status = RUNNABLE;
        push(run_queue, next_runner);
    }

    scheduler();

    in_lib = 0;
    return 0;
}

//Lock a mutex
int mypthread_mutex_lock(mypthread_mutex_t *mutex)
{
    in_lib = 1;
    while((*mutex)->locked)
    {
        threads[running_thread]->status = WAITING;
        enqueue((*mutex)->waiting_threads, threadnode_init(running_thread));
        mypthread_yield();
    }
    (*mutex)->locked = 1;
    (*mutex)->tid_locked_by = running_thread;
    in_lib = 0;
    return 0;
}

//Try to lock a mutex
int mypthread_mutex_trylock(mypthread_mutex_t *mutex)
{
    in_lib = 1;
    if((*mutex)->locked) return 1;
    (*mutex)->locked = 1;
    (*mutex)->tid_locked_by = running_thread;
    in_lib = 0;
    return 0;
}


//Internal functions

void scheduler()
{
    mypthread_t next_runner;
    mypthread_t curr = running_thread;
    ThreadNode *enqueued, *dequeued;

    //Enter current thread into run queue
    if(threads[curr]->status == RUNNING)
    {
        threads[curr]->status = RUNNABLE;
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
    } while(threads[next_runner]->status != RUNNABLE);

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
    ret->priority = DEFAULT_PRIORITY;
    ret->status = RUNNABLE;
    if(getcontext(&(ret->context)) == -1)
    {
        fprintf(stderr, "getcontext failed\n");
        return NULL;
    }
    stack = malloc(STACK_SIZE*sizeof(char));
    ret->context.uc_stack.ss_sp = stack;
    ret->context.uc_stack.ss_size = STACK_SIZE;
    ret->context.uc_link = NULL;

    return ret;
}

void thread_destroy(mypthread_t thread)
{
    ucontext_t context = threads[thread]->context;
    free(context.uc_stack.ss_sp);
    free(threads[thread]);
}

void timer_handler(int signum)
{
    if(in_lib)
    {
        return; //Do nothing if we are already in the library
    }
    in_lib = 1;
    scheduler();
}

void cleanup()
{
    int i;
    for(i = 0; i < thread_len; ++i)
    {
        if(threads[i] == NULL) break;
        thread_destroy(i);
    }
    free(threads);
    queue_destroy(run_queue);
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
