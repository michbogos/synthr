#ifndef WAVETABLE_H
#define WAVETABLE_H
#include<math.h>
typedef struct {
    int num_tables;
    int len;
    float sample_rate;
    int base;
    float** tables;
} Wavetable;
Wavetable wtbl_saw(float sample_rate, int table_len, int base);
Wavetable wtbl_tri(float sample_rate, int table_len, int base);
Wavetable wtbl_sqr(float sample_rate, int table_len, int base);
#endif