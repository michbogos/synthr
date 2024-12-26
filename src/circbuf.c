#include<circbuf.h>

circular_buffer init_circular_buffer(unsigned int item_size, unsigned int num_items){
    circular_buffer buf;
    buf.capacity = num_items;
    buf.item_size = item_size;
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

void write_circular_buffer(circular_buffer* buffer, void* ptr, unsigned int num_items){
    for(int i = 0; i < num_items*buffer->item_size; i++){
        buffer->write_ptr = buffer->write_ptr == buffer->end ? buffer->start : buffer->write_ptr;
        *((char*)buffer->write_ptr) = *((char*)ptr+i);
        buffer->write_ptr = ((char*)buffer->write_ptr)+1;
    }
    return;
}

void read_circularbuffer(circular_buffer* buffer, void* ptr, unsigned int num_items){
    for(int i = 0; i < num_items*buffer->item_size; i++){
        buffer->read_ptr = buffer->read_ptr == buffer->end ? buffer->start : buffer->read_ptr;
        *((char*)ptr+i) = *((char*)buffer->read_ptr);
        buffer->read_ptr = (char*)buffer->read_ptr+1;
    }
    return;
}