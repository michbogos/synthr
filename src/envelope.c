#include<envelope.h>

ADSREnvelope env_adsr(float attack, float delay, float sustain, float release){
    ADSREnvelope adsr;
    adsr.attack = attack;
    adsr.delay = delay;
    adsr.sustain = sustain;
    adsr.release = release;
    return adsr;
}

void gen_adsr_envelope(ADSREnvelope* adsr, float* output, int n, float sample_rate){
    for(int i = 0; i < n; i++){
        if(adsr->key_pressed && adsr->at < adsr->attack && adsr->dt < adsr->delay){
            adsr->dt = 0.0;
            adsr->rt = 0.0;
            adsr->at += 1.0f/sample_rate;
            output[i] = adsr->at/adsr->attack;
        }

        if(adsr->key_pressed && adsr->at-adsr->attack > -1.0f/sample_rate && adsr->dt-adsr->delay < -1.0f/sample_rate){
            adsr->dt += 1.0f/sample_rate;
            adsr->rt = 0.0;
            output[i] = 1.0f-((exp2f(adsr->dt/adsr->delay)/2.0f)*(1.0f-adsr->sustain));
        }

        if(adsr->key_pressed && adsr->at-adsr->attack > -1.0f/sample_rate && adsr->dt-adsr->delay > -1.0f/sample_rate){
            adsr->dt += 1.0f/sample_rate;
            adsr->rt = 0.0;
            output[i] = adsr->sustain;
        }

        if((!adsr->key_pressed) && adsr->rt == 0.0f){
            adsr->at = 0.0f;
            adsr->dt = 0.0f;
            adsr->release_value = output[i];
            adsr->rt += 1.0f/sample_rate;
        }

        if(!(adsr->key_pressed) && adsr->rt < adsr->release){
            adsr->dt = 0.0f;
            adsr->at = 0.0f;
            adsr->rt += 1.0f/sample_rate;
            output[i] = (exp2(-2*adsr->rt/adsr->release))*adsr->release_value;
        }

        if(!(adsr->key_pressed) && adsr->rt > adsr->release){
            output[i] = 0;
        }


    }
    return;
}