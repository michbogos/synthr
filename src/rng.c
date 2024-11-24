#include<rng.h>

unsigned int pcg32_random_r(pcg32_random_t* rng){
    unsigned long long oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    unsigned int xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    unsigned int rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

pcg32_random_t make_rng(unsigned long long initstate, unsigned long long initseq){
    pcg32_random_t rng;
    rng.state = 0U;
    rng.inc = (initseq << 1u) | 1u;
    pcg32_random_r(&rng);
    rng.state += initstate;
    pcg32_random_r(&rng);
    return rng;
}

float rand_float(pcg32_random_t* rng){
    return (float)pcg32_random_r(rng)/((float)__UINT32_MAX__);
}