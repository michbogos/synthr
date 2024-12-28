#include<delay.h>
#include<circbuf.h>

Delay init_delay(unsigned int delay_size, float decay){
    Delay d;
    d.buffer = init_circular_buffer(sizeof(float), 2*delay_size);
    float zero = 0;
    for(int i = 0; i < delay_size; i++){
        write_circular_buffer(&d.buffer, &zero, 1);
    }
    d.decay = decay;
    return d;
}

void delay(float* input, float* output, Delay * delay, int num_items){
    for(int i = 0; i < num_items; i++){
        float val = *(input+i)*delay->decay;
        write_circular_buffer(&delay->buffer, &val, 1);
    }
    read_circular_buffer(&delay->buffer, output, num_items);
}