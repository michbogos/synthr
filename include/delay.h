#ifndef DELAY_H
#define DELAY_H
#include<circbuf.h>

typedef struct {
    circular_buffer buffer;
    float decay;
    int delay_length;
    float delay_time;
} Delay;

Delay init_delay(unsigned int delay_size, float decay);

void delay(float* input, float* output, Delay* delay, int num_items);

void set_delay(Delay* delay, int num_smaples);

#endif