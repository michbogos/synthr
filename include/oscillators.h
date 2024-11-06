#ifndef OSCILLATORS
#define OSCILLATORS
#include<wavetable.h>
float osc_sqr(float* frequency, float* phase, float dt);
float osc_sin(float* frequency, float* phase, float dt);
float osc_saw(float* frequency, float* phase, float dt);
float osc_tri(float* frequency, float* phase, float dt);
float osc_tbl(float* frequency, float* phase, float dt, Wavetable* wavetable);
double poly_blep(float t, float dt);
#endif