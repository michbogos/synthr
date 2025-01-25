#ifndef ENVELOPE_H
#define ENVELOPE_H

#include<math.h>

typedef struct{
    float attack;
    float at;
    float delay;
    float dt;
    float sustain;
    float release;
    float rt;
    float release_value;
    int key_pressed;
} ADSREnvelope;

ADSREnvelope env_adsr(float attack, float delay, float sustain, float release);

void gen_adsr_envelope(ADSREnvelope* adsr, float* output, int n, float sample_rate);

#endif