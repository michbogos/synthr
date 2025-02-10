#include<envelope.h>

ADSREnvelope env_adsr(float ab, float ac, float db, float dc, float sustain, float rb, float rc){
    ADSREnvelope adsr;
    adsr.ab = ab;
    adsr.ac = ac;
    adsr.sustain = sustain;
    adsr.db = db;
    adsr.dc = dc;
    adsr.rb = rb;
    adsr.rc = rc;
    adsr.state = 0;
    return adsr;
}

float gen_adsr_envelope(ADSREnvelope* adsr){
    switch (adsr->state) {
        case 0:
            break;
        case 1:
            adsr->res = adsr->ab +  adsr->res * adsr->ac;
            if (adsr->res >= 1.0) {
                adsr->res = 1.0;
                adsr->state = 2;
            }
            break;
        case 2:
            adsr->res = adsr->db + adsr->res * adsr->dc;
            if (adsr->res <= adsr->sustain) {
                adsr->res = adsr->sustain;
                adsr->state = 3;
            }
            break;
        case 3:
            break;
        case 4:
            adsr->res = adsr->rb + adsr->res * adsr->rc;
            if (adsr->res <= 0.0) {
                adsr->res = 0.0;
                adsr->state = 0;
            }
    }
    return adsr->res;
}

void adsr_set_attack (ADSREnvelope* env, float t, float sample_rate){
    env->ac = powf(1.0f, sample_rate*t);
}
void adsr_set_delay  (ADSREnvelope* env, float t, float sample_rate);
void adsr_set_release(ADSREnvelope* env, float t, float sample_rate);

// void gen_adsr_envelope(ADSREnvelope* adsr, float* output, int n, float sample_rate){
//     for(int i = 0; i < n; i++){
//         if(adsr->key_pressed && adsr->at < adsr->attack && adsr->dt < adsr->delay){
//             adsr->dt = 0.0;
//             adsr->rt = 0.0;
//             adsr->at += 1.0f/sample_rate;
//             output[i] = adsr->at/adsr->attack;
//         }

//         if(adsr->key_pressed && adsr->at-adsr->attack > -1.0f/sample_rate && adsr->dt-adsr->delay < -1.0f/sample_rate){
//             adsr->dt += 1.0f/sample_rate;
//             adsr->rt = 0.0;
//             output[i] = 1.0f-((exp2f(adsr->dt/adsr->delay)/2.0f)*(1.0f-adsr->sustain));
//         }

//         if(adsr->key_pressed && adsr->at-adsr->attack > -1.0f/sample_rate && adsr->dt-adsr->delay > -1.0f/sample_rate){
//             adsr->dt += 1.0f/sample_rate;
//             adsr->rt = 0.0;
//             output[i] = adsr->sustain;
//         }

//         if((!adsr->key_pressed) && adsr->rt == 0.0f){
//             adsr->at = 0.0f;
//             adsr->dt = 0.0f;
//             adsr->release_value = output[i];
//             adsr->rt += 1.0f/sample_rate;
//         }

//         if(!(adsr->key_pressed) && adsr->rt < adsr->release){
//             adsr->dt = 0.0f;
//             adsr->at = 0.0f;
//             adsr->rt += 1.0f/sample_rate;
//             output[i] = (exp2(-2*adsr->rt/adsr->release))*adsr->release_value;
//         }

//         if(!(adsr->key_pressed) && adsr->rt > adsr->release){
//             output[i] = 0;
//         }


//     }
//     return;
// }