#ifndef OSCILLATORS
#define OSCILLATORS
#include<wavetable.h>
float osc_sqr(float frequency, float* phase, float dt);
float osc_sin(float frequency, float* phase, float dt);
float osc_saw(float frequency, float* phase, float dt);
float osc_tri(float frequency, float* phase, float dt);
float osc_ply(float n, float frequency, float* phase, float dt);
float osc_tbl(float frequency, float* phase, float dt, Wavetable* wavetable);
//float osc_tbl_chorus(float frequency, float cents, int n, float* phase, float dt, Wavetable* wavetable);
#endif