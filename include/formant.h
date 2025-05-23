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
        {800, 1150, 2900,3900,4950},  // Freq
        {0.0, -6.0, -32.0, -20.0, -50.0},  // Amp
        {80.0,90.0, 120.0, 130.0, 140.0},  // Bw
    },
    {                               // SOPRANO_E
        {350.0, 2000.0, 0.0, 0.0, 0.0},
        {0.0, -20.0, 0.0, 0.0, 0.0},
        {60.0, 100.0, 120.0, 0.0, 0.0},
    },
    {                               // SOPRANO_I
        {270.0, 2140.0, 0.0, 0.0, 0.0},
        {0.0, -12.0, 0.0, 0.0, 0.0},
        {90.0, 60.0, 0.0, 0.0, 0.0},
    },
    {                               // SOPRANO_O
        {450.0, 800.0, 0.0, 0.0, 0.0},
        {0.0, -11.0, 0.0, 0.0, 0.0},
        {70.0, 80.0, 0.0, 0.0, 0.0},
    },
    {                               // SOPRANO_U
        {250.0, 595.0, 0.0, 0.0, 0.0},
        {0.0, -16.0, 0.0, 0.0, 0.0},
        {50.0, 60.0, 0.0, 0.0, 0.0},
    },
    {
        {800.0, 1150.0, 0.0, 0.0, 0.0},
        {0.0, -4.0, 0.0, 0.0, 0.0},
        {80.0, 90.0, 0.0, 0.0, 0.0},
    },
    {
        {400.0, 1600.0, 0.0, 0.0, 0.0},
        {0.0, -24, 0.0, 0.0, 0.0},
        {60.0, 80.0, 0.0, 0.0, 0.0},
    },
    {
        {350.0, 1700.0, 0.0, 0.0, 0.0},
        {0.0, -20.0, 0.0, 0.0, 0.0},
        {50.0, 100.0, 0.0, 0.0, 0.0},
    },
    {
        {450.0, 800.0, 0.0, 0.0, 0.0},
        {0.0, -9.0, 0.0, 0.0, 0.0},
        {70.0, 80.0, 0.0, 0.0, 0.0},
    },
    {
        {325.0, 700.0, 0.0, 0.0, 0.0},
        {0.0, -12.0, 0.0, 0.0, 0.0},
        {50.0, 60.0, 0.0, 0.0, 0.0},
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
    for(int i = 0; i < 2; i++){
        f.freq[i] = data[0][i];
    }
    for(int i = 0; i < 2; i++){
        f.amp[i] = 24.0+data[1][i];
    }
    for(int i = 0; i < 2; i++){
        f.bw[i] = data[2][i];
    }
    for(int i = 0; i < 2; i++){
        f.filters[i] = biquad(PEAKEQ);
    }
    return f;
}

void formantize(float* input, int n, float sample_rate, Formant f){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < 2; j++){
            input[i] = filter(input[i], sample_rate, f.filters+j, f.freq[j], 3, f.amp[j]);
        }
    }
    return;
}
#endif