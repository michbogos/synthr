#include<delay.h>
#include<circbuf.h>

Delay init_delay(unsigned int delay_size, float decay){
    Delay d;
    d.delay_length = delay_size;
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

void set_delay(Delay * delay, int num_samples){
    int diff = delay->delay_length-num_samples;
    if(diff > 0){
        int len = (int)(delay->buffer.end-delay->buffer.start);
        delay->buffer.read_ptr = delay->buffer.start+(((int)(delay->buffer.read_ptr-delay->buffer.start)+(int)(diff*delay->buffer.item_size))%len);
        //delay->buffer.read_ptr = delay->buffer.start+(((int)delay->buffer.read_ptr-(int)delay->buffer.start+(diff*delay->buffer.item_size))%(int)delay->buffer.end-(int)delay->buffer.end);
    }
    delay->delay_length = num_samples;
}