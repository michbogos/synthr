#include<oscillators.h>
#include<wavetable.h>
#include<math.h>

#define PI 3.1415926

float osc_sqr(int n, float sample_rate, float frequency){
    return (((int)((float)n*(float)(frequency/sample_rate))%2)-0.5)*2;
}

float osc_sin(int n, float sample_rate, float frequency){
    return sin((float)2*PI*frequency*n/sample_rate);
}

float osc_saw(int n, float sample_rate, float frequency){
    float val = -(float)(frequency/2)*n/sample_rate;
    return ((val-floorf(val))-0.5)*2;
}


//Triangle wave phase shifts due to float to int rounding
float osc_tri(int n, float sample_rate, float frequency){
    int val = (n % (int)((float)sample_rate/frequency)) - ((int)((float)sample_rate/frequency))/2.0f;
    val = val > 0 ? val : -val;
    return val/(float)(sample_rate/frequency)-0.5;
}

float osc_tbl(int n, float sample_rate, float frequency, Wavetable* wavetable){
    int table_idx = (int)floor(log2(frequency/wavetable->base)) < wavetable->num_tables ? (int)floor(log2(frequency/wavetable->base)): wavetable->num_tables-1;
    table_idx = table_idx < 0 ? 0 : table_idx;
    float* table = wavetable->tables[table_idx];
    float idx = ((float)n/(float)(sample_rate/frequency));
    float subidx = (idx-floor(idx))*wavetable->len;
    float t = subidx-floor(subidx);
    float s1 = table[(int)floor(subidx)];
    float s2 = table[(int)ceil(subidx)];
    // float t = ((float)n/sample_rate)*frequency-floor(((float)n/sample_rate)*frequency);
    return s1+(s2-s1)*t;
}