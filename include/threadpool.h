#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<pthread.h>
#include<queue.h>

typedef struct{
    int num_threads;
    int num_active;
    Queue* queue;
}Threadpool;

#endif