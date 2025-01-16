#include<fft.h>

int reverse(int num, int lg_n) {
    int res = 0;
    for (int i = 0; i < lg_n; i++) {
        if (num & (1 << i))
            res |= 1 << (lg_n - 1 - i);
    }
    return res;
}

void fft(complex* x, int N){
    int log_n = 0;
    while((1<<log_n) < N){
        log_n ++;
    }
    for(int i = 0; i < N; i++){
        int rev = reverse(i, log_n);
        if(i < rev){
            float tmp = x[i];
            x[i] = x[rev];
            x[rev] = tmp;
        }
    }
    for (int len = 2; len <= N; len <<= 1) {
        double ang = 2 * PI / len;
        complex wlen = cos(ang)+I*sin(ang);
        for (int i = 0; i < N; i += len) {
            complex w = 1.0;
            for (int j = 0; j < len / 2; j++) {
                complex u = x[i+j];
                complex v = x[i+j+len/2] * w;
                x[i+j] = u + v;
                x[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }
}