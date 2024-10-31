#include<wavetable.h>

void wtbl_saw(float* tbl, float sampling_rate){
    for(int i = 0; i < WAVETABLE_SIZE; i++){
        tbl[i] = 0.5;
        for(int k = 1; k < 16; k++){
            tbl[i] -= k%2 ? (1.0/3.1415926)*(1.0/k)*sin(2*3.1415926*k*((float)i/WAVETABLE_SIZE)) : (1.0/3.1415926)*(-1.0/k)*sin(2*3.1415926*k*((float)i/WAVETABLE_SIZE));
        }
    }
} 