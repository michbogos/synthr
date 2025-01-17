#include<fft.h>

void fft(complex* x, int N, int invert){
    int log_n = 0;
    while((1<<log_n) < N){
        log_n ++;
    }
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j){
            complex tmp = x[i];
            x[i] = x[j];
            x[j] = tmp;
        }
    }
    for (int len = 2; len <= N; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        complex wlen = cos(ang)+I*sin(ang);
        for (int i = 0; i < N; i += len) {
            complex w = 1.0;
            for (int j = 0; j < len / 2; j++){
                complex u = x[i+j];
                complex v = x[i+j+len/2] * w;
                x[i+j] = u + v;
                x[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for(int i = 0; i < N; i++)
            x[i] /= N;
    }
}