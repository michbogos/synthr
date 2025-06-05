#ifndef KARPLUS
#define KARPLUS

#include<delay.h>
#include<filter.h>
#include<rng.h>

typedef struct KarplusState{
    Delay delay;
    Biquad filter;
    pcg32_random_t rng;
    int noise_counter;
    float sample_rate;
    float freq;
} KarplusState;

KarplusState karplus_init(float sample_rate){
    KarplusState state;
    state.delay = init_delay((int)sample_rate, 1.0f);
    state.filter = biquad(LOWPASS);
    state.sample_rate = sample_rate;
    state.rng = make_rng(124313, 23421);
    return state;
}

void karplus_trigger(KarplusState* state, float frequency){
    int d = state->sample_rate/frequency;
    set_delay(&state->delay, d);
    state->noise_counter += 100;
    state->freq = frequency;
}

float karplus_compute(KarplusState* state){
    float output = 0;
    if(state->noise_counter > 0){
        output += (rand_float(&state->rng)-0.5)*4;
        state->noise_counter--;
    }
    float delayed;
    delay(&output, &delayed, &state->delay, 1);
    output += filter(delayed*2, state->sample_rate, &state->filter, state->freq*1, 100, 0);
    return output;
}

#endif