#ifndef _MYPTHREAD_H
#define _MYPTHREAD_H 1

//Includes

#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <sys/time.h>


//Defines

enum ThreadState {
    RUNNING,
    WAITING,
    KILLED,
    NEW
};

typedef enum ThreadState ThreadState;

//#define RUNNING  1
//#define WAITING  2
//#define KILLED   3

#define STACK_SIZE 16384

//Typedefs

typedef int mypthread_t;
typedef void *(*thread_func)(void *);

typedef struct _mypthread_t {
    int tid;
    ThreadState status;
    ucontext_t context;
    thread_func f;
    void *retval;
} _mypthread_t;

typedef struct _mypthread_mutex_t {
    char locked;
    int tid_locked_by;
} _mypthread_mutex_t;

typedef struct _mypthread_mutex_t * mypthread_mutex_t;

typedef struct ThreadNode {
    mypthread_t thread;
    struct ThreadNode *next;
} ThreadNode;

typedef struct Queue {
    ThreadNode *front;
    ThreadNode *back;
    int length;
} Queue;

//Function definitions - user facing
void mypthread_create(mypthread_t *thread, char *garbage, thread_func, void *args);
void mypthread_yield();
void mypthread_join(mypthread_t thread, void **ret);
void mypthread_exit(void *ret);

//Function definitions - locks
int mypthread_mutex_init(mypthread_mutex_t *, char *);
int mypthread_mutex_destroy(mypthread_mutex_t *);
int mypthread_mutex_unlock(mypthread_mutex_t *);
int mypthread_mutex_lock(mypthread_mutex_t *);
int mypthread_mutex_trylock(mypthread_mutex_t *);

//Function definitions - internal use
void scheduler();
_mypthread_t *thread_init(mypthread_t);
void timer_handler(int);

//Function definitions - queue
Queue *queue_init();
void queue_destroy();
void push(Queue *, ThreadNode *);
void enqueue(Queue *, ThreadNode *);
ThreadNode *dequeue(Queue *);

ThreadNode *threadnode_init(mypthread_t);

#endif
