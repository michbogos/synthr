#ifndef OSCILLATORS
#define OSCILLATORS
#include<wavetable.h>
float osc_sqr(int n, float sample_rate, float frequency);
float osc_sin(int n, float sample_rate, float frequency);
float osc_saw(int n, float sample_rate, float frequency);
float osc_tri(int n, float sample_rate, float frequency);
float osc_tbl(int n, float sample_rate, float frequency, Wavetable* wavetable);
#endif