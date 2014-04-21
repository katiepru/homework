#ifndef _MYPTHREAD_H
#define _MYPTHREAD_H 1

//Includes

#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>


//Defines

#define RUNNING  1
#define WAITING  2
#define KILLED   3
#define NEW      4

#define STACK_SIZE 16384

//Typedefs

typedef void *(*thread_func)(void *);
typedef struct mypthread_t {
    int tid;
    int status;
    ucontext_t context;
    thread_func f;
} mypthread_t;

typedef struct ThreadNode {
    mypthread_t *thread;
    struct ThreadNode *next;
} ThreadNode;

typedef struct Queue {
    ThreadNode *front;
    ThreadNode *back;
    int length;
} Queue;

//Function definitions - user facing

void mypthread_create(mypthread_t *thread, thread_func, void *args);
void mypthread_yield();
void mypthread_join(mypthread_t thread, void **ret);
void mypthread_exit(void *ret);

//Function definitions - internal use
void scheduler();

//Function definitions - queue
Queue *queue_init();
void queue_destroy();
void enqueue(Queue *, ThreadNode *);
ThreadNode *dequeue(Queue *);

#endif
