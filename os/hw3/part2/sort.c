#include <stdio.h>
#include <stdlib.h>
#include "mypthread.h"
#include "sort.h"

char done = 0;

int main(int argc, char *argv[])
{
    mypthread_t sorters[argc-2];
    mypthread_t checker;
    mypthread_mutex_t mutexes[argc-1];
    arg_struct *args;
    check_args *cargs;

    int nums[argc-1];
    int i;

    //Populate array to sort and init mutexes
    for(i = 1; i < argc; ++i)
    {
        nums[i-1] = atoi(argv[i]);
        mypthread_mutex_init(&(mutexes[i-1]), NULL);
    }

    //Start sorter threads
    for(i = 0; i < argc-2; ++i)
    {
        //Build arg struct
        args = malloc(sizeof(arg_struct));
        args->arr = nums;
        args->left = &(mutexes[i]);
        args->right = &(mutexes[i+1]);
        args->start_ind = i;

        //Create thread
        mypthread_create(&(sorters[i]), NULL, sort, (void *) args);
    }

    cargs = malloc(sizeof(cargs));
    cargs->arr = nums;
    cargs->mutexes = mutexes;
    cargs->length = argc-1;

    while(!done)
    {
        mypthread_create(&checker, NULL, check, (void *) cargs);
        mypthread_join(checker, NULL);
    }

    for(i = 0; i < argc-2; ++i)
    {
        mypthread_join(sorters[i], NULL);
    }

    for(i = 0; i < argc-1; ++i)
    {
        printf("%d\n", nums[i]);
    }

    return 0;
}

void *sort(void *args)
{
    arg_struct *a = (arg_struct *) args;

    int *arr = a->arr;
    mypthread_mutex_t *left = a->left;
    mypthread_mutex_t *right = a->right;
    int start_ind = a->start_ind;

    int tmp;

    while(!done)
    {
        /*        if(start_ind % 2 == 1)
                  {
                  mypthread_mutex_lock(right);
                  mypthread_mutex_lock(left);

                  if(arr[start_ind] < arr[start_ind+1])
                  {
                  tmp = arr[start_ind];
                  arr[start_ind] = arr[start_ind+1];
                  arr[start_ind+1] = tmp;
                  }

                  } else
                  {*/
        mypthread_mutex_lock(left);
        mypthread_mutex_lock(right);

        if(arr[start_ind] < arr[start_ind+1])
        {
            tmp = arr[start_ind];
            arr[start_ind] = arr[start_ind+1];
            arr[start_ind+1] = tmp;
        }
        // }
        mypthread_mutex_unlock(right);
        mypthread_mutex_unlock(left);
    }

    mypthread_exit((void *)(size_t)34);
    return (void *)(size_t)33;
}

void *check(void *args)
{
    check_args *cargs = (check_args *) args;
    int *arr = cargs->arr;
    int length = cargs->length;
    mypthread_mutex_t *mutexes = cargs->mutexes;

    int prev;
    int i;
    int bad = 0;

    mypthread_mutex_lock(&(mutexes[0]));
    prev = arr[0];
    mypthread_mutex_unlock(&(mutexes[0]));

    for(i = 1; i < length; ++i)
    {
        mypthread_mutex_lock(&(mutexes[i]));
        if(arr[i] > prev)
            bad = 1;
        prev = arr[i];
        mypthread_mutex_unlock(&(mutexes[i]));

       if(bad)
        {
            mypthread_exit((void *)(size_t)15);
        }
    }
    done = 1;

    mypthread_exit((void *)(size_t)14);
    return (void *)(size_t)13;
}
