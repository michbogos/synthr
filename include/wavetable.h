#ifndef WAVETABLE
#define WAVETABLE
#define WAVETABLE_SIZE 1024
#include<math.h>
typedef struct {
    int num_tables;
    int len;
    float sample_rate;
    int base;
    float** tables;
} Wavetable;
Wavetable wtbl_saw(float sample_rate, int table_len, int base);
#endif