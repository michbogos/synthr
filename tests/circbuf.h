#pragma once
#ifndef CIRCBUF
#define CIRCBUF
#include<stdlib.h>

typedef struct {
    void* start;
    void* end;
    void* write_ptr;
    void* read_ptr;
    unsigned int item_size;
    unsigned int capacity;
    unsigned int count;
} circular_buffer;

circular_buffer init_circular_buffer(unsigned int item_size, unsigned int num_items);
void free_circular_buffer(circular_buffer* buffer);
void write_circular_buffer(circular_buffer* buffer, void* ptr, unsigned int num_items);
void read_circularbuffer(circular_buffer* buffer, void* ptr, unsigned int num_items);

#endif