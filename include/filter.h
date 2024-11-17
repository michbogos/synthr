#ifndef FILTER_H
#define FILTER_H

struct filterData{
    float out1;
    float out2;
    float out3;
    float out4;
    float in1;
    float in2;
    float in3;
    float in4;
};

float filter_lowpass(float input, struct filterData* data, double fc, double res);

#endif