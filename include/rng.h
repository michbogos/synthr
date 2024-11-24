#ifndef PCG_RANDOM_H
#define PCG_RANDOM_H

typedef struct { unsigned long long state;  unsigned long long inc; } pcg32_random_t;

unsigned int pcg32_random_r(pcg32_random_t* rng);

pcg32_random_t make_rng(unsigned long long initstate, unsigned long long initseq);

float rand_float(pcg32_random_t* rng);

#endif