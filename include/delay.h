#ifndef DELAY_H
#define DELAY_H
#include<circbuf.h>

typedef struct {
    circular_buffer buffer;
    float decay;
    float delay_time;
} Delay;

Delay init_delay(unsigned int delay_size, float decay);

void delay(float* input, float* output, Delay* delay, int num_items);

#endif