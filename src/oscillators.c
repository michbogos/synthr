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
    float val = -(float)(frequency/2)*n/sample_rate;
    return ((val-floorf(val))-0.5)*2;
}


//Triangle wave phase shifts due to float to int rounding
float osc_tri(int n, float sample_rate, float frequency){
    int val = (n % (int)((float)sample_rate/frequency)) - ((int)((float)sample_rate/frequency))/2.0f;
    val = val > 0 ? val : -val;
    return val/(float)(sample_rate/frequency)-0.5;
}