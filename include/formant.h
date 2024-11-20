#ifndef FORMANT_H
#define FORMANT_H
#include<filter.h>
#include<math.h>

enum FormantType{
    SOPRANO_A,
    SOPRANO_E,
    SOPRANO_I,
    SOPRANO_O,
    SOPRANO_U,
    ALTO_A,
    ALTO_E,
    ALTO_I,
    ALTO_O,
    ALTO_U,
    COUNTERTENOR_A,
    COUNTERTENOR_E,
    COUNTERTENOR_I,
    COUNTERTENOR_O,
    COUNTERTENOR_U,
    TENOR_A,
    TENOR_E,
    TENOR_I,
    TENOR_O,
    TENOR_U,
    BASS_A,
    BASS_E,
    BASS_I,
    BASS_O,
    BASS_U,
};

struct frm{
    float freq[5];
    float amp[5];
    float bw[5];
    Biquad filters[5];
};

typedef struct frm Formant;

float FORMANT_TABLE[20][3][5] = {
    {                               // SOPRANO_A
        {450,800,2830,2650,2800},  // Freq
        {10.0, -1.0, -12.0, -28.0, -55.0},  // Amp
        {70.0,80.0, 100.0, 130.0, 135.0},  // Bw
    },
    {                               // SOPRANO_E
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {                               // SOPRANO_I
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {                               // SOPRANO_O
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {                               // SOPRANO_U
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
    {
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0},
    },
};

Formant make_formant(float data[3][5]){
    Formant f;
    for(int i = 0; i < 5; i++){
        f.freq[i] = data[0][i];
    }
    for(int i = 0; i < 5; i++){
        f.amp[i] = data[1][i];
    }
    for(int i = 0; i < 5; i++){

        f.bw[i] = data[2][i];
    }
    for(int i = 0; i < 5; i++){
        f.filters[i] = biquad(PEAKEQ);
    }
    return f;
}

float formantize(float sample_rate, float input, Formant f){
    for(int i = 0; i < 5; i++){
        float w0 = 2*PI*f.freq[i]/sample_rate;
        float Q = 2*sinh(log(2)/2*f.bw[i]*w0/sin(w0));
        input = filter(input, sample_rate, f.filters+i, f.freq[i], Q, f.amp[i]);
    }
    return input;
}
#endif