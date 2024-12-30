#pragma once
#ifndef FILTER_H
#define FILTER_H
#include<defs.h>
#include<circbuf.h>

enum BiquadType{
    LOWPASS, HIGHPASS, BANDPASSQ, BANDPASS0, NOTCH, APF, PEAKEQ, LOWSHELF, HIGHSHELF
};

struct f{
    float* in;
    float* out;
    float* a;
    float* b;
    int numa;
    int numb;
};

typedef struct f Filter;

struct bq{
    float in1;
    float in2;
    float in3;
    float out1;
    float out2;
    float out3;
    enum BiquadType type;
};

typedef struct bq Biquad;

typedef struct {
    circular_buffer buffer;
    int n;
    float alpha;
} CombFilter;


typedef struct {
    circular_buffer buffer;
    int n;
    float feedback;
} AllpassFilter;

// Filter filter_lowpass(){
//     Filter f;
//     f.a = NULL;
//     f.b = NULL;
//     f.in = malloc(4*sizeof(float));
//     f.out = malloc(4*sizeof(float));
//     f.numa = 0;
//     f.numb = 0;
//     return f;
// };

Biquad biquad(enum BiquadType type);
CombFilter comb(int n, float alpha);
AllpassFilter init_all_pass(int n, float alpha);

Filter filter_coeffs(float* a, float* b, int numa, int numb);
float filter_comb(CombFilter* filter, float sample);
float filter_allpass(AllpassFilter* filter, float sample);

float filter(float input, float fs, Biquad* filter, float fc, float Q, float dBgain);

#endif