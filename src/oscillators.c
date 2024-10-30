#include<oscillators.h>
#include<math.h>

#define SAMPLE_RATE 44100
#define PI 3.1415926

float osc_sqr(int n, float sample_rate, float frequency){
    return (((int)((float)n*(float)(frequency/sample_rate))%2)-0.5)*2;
}

float osc_sin(int n, float sample_rate, float frequency){
    return sin((float)2*PI*frequency*n/sample_rate);
}

float osc_saw(int n, float sample_rate, float frequency){
    float val = -(float)frequency*n/sample_rate;
    return ((val-floorf(val))-0.5)*2;
}

float osc_tri(int n, float sample_rate, float frequency){
    float val = (float)frequency*n/sample_rate;
    int sign = (int)((float)(n*((float)frequency/sample_rate)))%2 ? 1 : -1;
    return ((sign*(val-floorf(val)))-0.5*sign)*2;
}