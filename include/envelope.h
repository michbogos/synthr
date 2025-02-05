#ifndef ENVELOPE_H
#define ENVELOPE_H

#include<math.h>

typedef struct{
    float ab;
    float ac;
    float db;
    float dc;
    float sustain;
    float rb;
    float rc;
    float res;
    int state;
    float gate;
} ADSREnvelope;

ADSREnvelope env_adsr(float ab, float ac, float db, float dc, float sustain, float rb, float rc);

float gen_adsr_envelope(ADSREnvelope* adsr);

#endif