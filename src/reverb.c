#include<reverb.h>
#include<filter.h>

const float FIXED_GAIN = 0.015;

const float SCALE_WET = 3.0;
const float SCALE_DAMPENING = 0.4;

const float SCALE_ROOM = 0.28;
const float OFFSET_ROOM = 0.5;

const unsigned int STEREO_SPREAD = 23;

const unsigned int COMB_TUNING_L1 = 1116;
const unsigned int COMB_TUNING_R1 = 1116 + STEREO_SPREAD;
const unsigned int COMB_TUNING_L2 = 1188;
const unsigned int COMB_TUNING_R2 = 1188 + STEREO_SPREAD;
const unsigned int COMB_TUNING_L3 = 1277;
const unsigned int COMB_TUNING_R3 = 1277 + STEREO_SPREAD;
const unsigned int COMB_TUNING_L4 = 1356;
const unsigned int COMB_TUNING_R4 = 1356 + STEREO_SPREAD;
const unsigned int COMB_TUNING_L5 = 1422;
const unsigned int COMB_TUNING_R5 = 1422 + STEREO_SPREAD;
const unsigned int COMB_TUNING_L6 = 1491;
const unsigned int COMB_TUNING_R6 = 1491 + STEREO_SPREAD;
const unsigned int COMB_TUNING_L7 = 1557;
const unsigned int COMB_TUNING_R7 = 1557 + STEREO_SPREAD;
const unsigned int COMB_TUNING_L8 = 1617;
const unsigned int COMB_TUNING_R8 = 1617 + STEREO_SPREAD;

const unsigned int ALLPASS_TUNING_L1 = 556;
const unsigned int ALLPASS_TUNING_R1 = 556 + STEREO_SPREAD;
const unsigned int ALLPASS_TUNING_L2 = 441;
const unsigned int ALLPASS_TUNING_R2 = 441 + STEREO_SPREAD;
const unsigned int ALLPASS_TUNING_L3 = 341;
const unsigned int ALLPASS_TUNING_R3 = 341 + STEREO_SPREAD;
const unsigned int ALLPASS_TUNING_L4 = 225;
const unsigned int ALLPASS_TUNING_R4 = 225 + STEREO_SPREAD;

unsigned int adjust_length(unsigned int length, float sr){
    return (unsigned int)((float)length*sr/44100.0f);
}

Reverb init_reverb(float dry,float wet,float gain,float width,float room_size,float dampening, float sample_rate){
    Reverb reverb;
    reverb.combs[0] = comb(adjust_length(COMB_TUNING_L1, sample_rate), 1.0f, 1.0f);
    reverb.combs[1] = comb(adjust_length(COMB_TUNING_R1, sample_rate), 1.0f, 1.0f);

    reverb.combs[2] = comb(adjust_length(COMB_TUNING_L2, sample_rate), 1.0f, 1.0f);
    reverb.combs[3] = comb(adjust_length(COMB_TUNING_R2, sample_rate), 1.0f, 1.0f);

    reverb.combs[4] = comb(adjust_length(COMB_TUNING_L3, sample_rate), 1.0f, 1.0f);
    reverb.combs[5] = comb(adjust_length(COMB_TUNING_R3, sample_rate), 1.0f, 1.0f);

    reverb.combs[6] = comb(adjust_length(COMB_TUNING_L4, sample_rate), 1.0f, 1.0f);
    reverb.combs[7] = comb(adjust_length(COMB_TUNING_R4, sample_rate), 1.0f, 1.0f);

    reverb.combs[8] = comb(adjust_length(COMB_TUNING_L5, sample_rate), 1.0f, 1.0f);
    reverb.combs[9] = comb(adjust_length(COMB_TUNING_R5, sample_rate), 1.0f, 1.0f);

    reverb.combs[10] = comb(adjust_length(COMB_TUNING_L6, sample_rate), 1.0f, 1.0f);
    reverb.combs[11] = comb(adjust_length(COMB_TUNING_R6, sample_rate), 1.0f, 1.0f);

    reverb.combs[12] = comb(adjust_length(COMB_TUNING_L7, sample_rate), 1.0f, 1.0f);
    reverb.combs[13] = comb(adjust_length(COMB_TUNING_R7, sample_rate), 1.0f, 1.0f);

    reverb.combs[14] = comb(adjust_length(COMB_TUNING_L8, sample_rate), 1.0f, 1.0f);
    reverb.combs[15] = comb(adjust_length(COMB_TUNING_R8, sample_rate), 1.0f, 1.0f);


    reverb.allpass[0] = init_all_pass(adjust_length(ALLPASS_TUNING_L1, sample_rate), 0.5f);
    reverb.allpass[1] = init_all_pass(adjust_length(ALLPASS_TUNING_R1, sample_rate), 0.5f);

    reverb.allpass[2] = init_all_pass(adjust_length(ALLPASS_TUNING_L2, sample_rate), 0.5f);
    reverb.allpass[3] = init_all_pass(adjust_length(ALLPASS_TUNING_R2, sample_rate), 0.5f);

    reverb.allpass[4] = init_all_pass(adjust_length(ALLPASS_TUNING_L3, sample_rate), 0.5f);
    reverb.allpass[5] = init_all_pass(adjust_length(ALLPASS_TUNING_R3, sample_rate), 0.5f);

    reverb.allpass[6] = init_all_pass(adjust_length(ALLPASS_TUNING_L4, sample_rate), 0.5f);
    reverb.allpass[7] = init_all_pass(adjust_length(ALLPASS_TUNING_R4, sample_rate), 0.5f);

    reverb.dampening = dampening;
    reverb.dry = dry;
    reverb.gain = gain;
    reverb.room_size = room_size;
    reverb.wet = wet;
    reverb.width = width;

    return reverb;
}

void reverb(Reverb* r, float* input, float* output, int n, int stereo){
    float outl = 0.0;
    float outr = 0.0;
    if(!stereo){
        for(int i = 0; i < n; i++){
            for(int comb = 0; comb < 8; comb++){
                outl += filter_comb(r->combs+(comb*2+0), input[i]);
                outr += filter_comb(r->combs+(comb*2+1), input[i]);
            }
            for(int allpass = 0; allpass < 4; allpass++){
                outl += filter_allpass(r->allpass+2*allpass+0, outl);
                outr += filter_allpass(r->allpass+2*allpass+1, outr);
            }
            output[i] = ((outl+outr)/2)*4;
        }
    }
}

