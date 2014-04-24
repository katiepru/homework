void *sort(void*);
void *check(void*);

typedef struct args {
    int *arr;
    mypthread_mutex_t *left;
    mypthread_mutex_t *right;
    int start_ind;
} arg_struct;

typedef struct check_args {
    int *arr;
    int length;
    mypthread_mutex_t *mutexes;
} check_args;

