//http://musicweb.ucsd.edu/~trsmyth/papers/KSExtensions.pdf


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
    float prev_output;
    float prev_x;
    float prev_y;
    float tune;
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
    float finetune = (((((float)state->sample_rate/(float)frequency)))-d)-0.5;
    state->tune = (1-finetune)/(1+finetune);
    // state->tune = 1;
    state->noise_counter += 100;
    state->freq = frequency;
}

//Out of tune because delay is 0.5 longer than d
float karplus_compute(KarplusState* state){
    if(state->noise_counter > 0){
        state->prev_output += (rand_float(&state->rng)-0.5)*2;
        state->noise_counter--;
    }
    float delayed;
    delay(&state->prev_output, &delayed, &state->delay, 1);
    state->prev_output = (state->prev_output+delayed)/2.0f;//filter(delayed, state->sample_rate, &state->filter, state->freq*6, 0.7f, 0);
    state->prev_y = state->prev_output*state->tune+state->prev_x-state->prev_y*state->tune;
    state->prev_x = state->prev_output;
    state->prev_output = state->prev_y;
    //delay(&output, &delayed, &state->delay, 1);
    //state->prev_output = output;
    return state->prev_output;
}

#endif