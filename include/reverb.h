#ifndef REVERB_H
#define REVERB_H
#include<filter.h>

typedef struct{
    CombFilter combs[16];
    AllpassFilter allpass[8];
    float dry;
    float wet;
    float gain;
    float width;
    float room_size;
    float dampening;
} Reverb;

Reverb init_reverb(float dry,float wet,float gain,float width,float room_size,float dampening, float sample_rate);

void reverb(Reverb* r, float* input, float* output, int n, int stereo);
#endif