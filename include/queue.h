#ifndef QUEUE_H
#define QUEUE_H

typedef struct{
    void* data;
    unsigned int data_size;
    Queue* next;
}Queue;

#endif