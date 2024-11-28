typedef struct{
    float attack;
    float at;
    float delay;
    float dt;
    float sustain;
    float release;
    float rt;
    int key_pressed;
} ADSREnvelope;

ADSREnvelope env_adsr(float attack, float delay, float sustain, float release){
    ADSREnvelope adsr;
    adsr.attack = attack;
    adsr.delay = delay;
    adsr.sustain = sustain;
    adsr.release = release;
    return adsr;
}

void gen_adsr_envelope(ADSREnvelope* adsr, float* output, int n, float sample_rate){
    return;
}