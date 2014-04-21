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
    mypthread_t prev_running;
    char *main_thread_stack, *thread_stack;

    if(!init)
    {
        init = 1;
        threads = calloc(512, sizeof(_mypthread_t *));

        //Init run queue
        run_queue = queue_init();

        //Create main thread
        main_thread = malloc(sizeof(_mypthread_t));
        threads[0] = main_thread;
        main_thread->tid = 0;
        main_thread->retval = NULL;
        main_thread->status = NEW;
        if(getcontext(&(main_thread->context)) == -1)
        {
            fprintf(stderr, "getcontext failed\n");
            exit(5);
        }
        main_thread_stack = malloc(STACK_SIZE*sizeof(char));
        main_thread->context.uc_stack.ss_sp = main_thread_stack;
        main_thread->context.uc_stack.ss_size = STACK_SIZE;
        //FIXME: Successor
        main_thread->status = RUNNING;
        running_thread = 0;

    }


    thread = malloc(sizeof(mypthread_t));
    *threadID = tid++;
    thread->tid = *threadID;
    threads[*threadID] = thread;
    thread->status = NEW;
    thread->retval = NULL;

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
    prev_running = running_thread;
    running_thread = *threadID;
    threads[prev_running]->status = WAITING;
    enqueue(run_queue, threadnode_init(prev_running));
    thread->status = RUNNING;

    swapcontext(&(threads[prev_running]->context), &(thread->context));
    return;
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
        printf("Callinf yeild\n");
        mypthread_yield();
        printf("Waking up\n");
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
    if(threads[curr]->status == RUNNING)
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
    swapcontext(&(threads[curr]->context), &(threads[next_runner]->context));

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
