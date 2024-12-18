#ifndef CIRCBUF
#define CIRCBUF
#include<stdlib.h>

typedef struct {
    void* start;
    void* end;
    void* write_ptr;
    void* read_ptr;
    unsigned int size;
    unsigned int capacity;
    unsigned int count;
} circular_buffer;

circular_buffer init_circular_buffer(unsigned int item_size, unsigned int num_items){
    circular_buffer buf;
    buf.capacity = num_items;
    buf.count = 0;
    buf.start = malloc(item_size*num_items);
    buf.end = buf.start+item_size*num_items;
    buf.write_ptr = buf.start;
    buf.read_ptr = buf.start;
    return buf;
}

void free_circular_buffer(circular_buffer* buffer){
    free(buffer->start);
    return;
}

#endif