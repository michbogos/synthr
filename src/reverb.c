// #include<reverb.h>
// #include<filter.h>

// const float FIXED_GAIN = 0.015;

// const float SCALE_WET = 3.0;
// const float SCALE_DAMPENING = 0.4;

// const float SCALE_ROOM = 0.28;
// const float OFFSET_ROOM = 0.5;

// const unsigned int STEREO_SPREAD = 23;

// const unsigned int COMB_TUNING_L1 = 1116;
// const unsigned int COMB_TUNING_R1 = 1116 + STEREO_SPREAD;
// const unsigned int COMB_TUNING_L2 = 1188;
// const unsigned int COMB_TUNING_R2 = 1188 + STEREO_SPREAD;
// const unsigned int COMB_TUNING_L3 = 1277;
// const unsigned int COMB_TUNING_R3 = 1277 + STEREO_SPREAD;
// const unsigned int COMB_TUNING_L4 = 1356;
// const unsigned int COMB_TUNING_R4 = 1356 + STEREO_SPREAD;
// const unsigned int COMB_TUNING_L5 = 1422;
// const unsigned int COMB_TUNING_R5 = 1422 + STEREO_SPREAD;
// const unsigned int COMB_TUNING_L6 = 1491;
// const unsigned int COMB_TUNING_R6 = 1491 + STEREO_SPREAD;
// const unsigned int COMB_TUNING_L7 = 1557;
// const unsigned int COMB_TUNING_R7 = 1557 + STEREO_SPREAD;
// const unsigned int COMB_TUNING_L8 = 1617;
// const unsigned int COMB_TUNING_R8 = 1617 + STEREO_SPREAD;

// const unsigned int ALLPASS_TUNING_L1 = 556;
// const unsigned int ALLPASS_TUNING_R1 = 556 + STEREO_SPREAD;
// const unsigned int ALLPASS_TUNING_L2 = 441;
// const unsigned int ALLPASS_TUNING_R2 = 441 + STEREO_SPREAD;
// const unsigned int ALLPASS_TUNING_L3 = 341;
// const unsigned int ALLPASS_TUNING_R3 = 341 + STEREO_SPREAD;
// const unsigned int ALLPASS_TUNING_L4 = 225;
// const unsigned int ALLPASS_TUNING_R4 = 225 + STEREO_SPREAD;

// unsigned int adjust_length(unsigned int length, float sr){
//     return (unsigned int)((float)length*sr/44100.0f);
// }

// Reverb init_reverb(float dry,float wet,float gain,float width,float room_size,float dampening, float sample_rate){
//     Reverb reverb;
//     reverb.combs[0] = comb(adjust_length(COMB_TUNING_L1, sample_rate), 1.0f, 1.0f);
//     reverb.combs[1] = comb(adjust_length(COMB_TUNING_R1, sample_rate), 1.0f, 1.0f);

//     reverb.combs[2] = comb(adjust_length(COMB_TUNING_L2, sample_rate), 1.0f, 1.0f);
//     reverb.combs[3] = comb(adjust_length(COMB_TUNING_R2, sample_rate), 1.0f, 1.0f);

//     reverb.combs[4] = comb(adjust_length(COMB_TUNING_L3, sample_rate), 1.0f, 1.0f);
//     reverb.combs[5] = comb(adjust_length(COMB_TUNING_R3, sample_rate), 1.0f, 1.0f);

//     reverb.combs[6] = comb(adjust_length(COMB_TUNING_L4, sample_rate), 1.0f, 1.0f);
//     reverb.combs[7] = comb(adjust_length(COMB_TUNING_R4, sample_rate), 1.0f, 1.0f);

//     reverb.combs[8] = comb(adjust_length(COMB_TUNING_L5, sample_rate), 1.0f, 1.0f);
//     reverb.combs[9] = comb(adjust_length(COMB_TUNING_R5, sample_rate), 1.0f, 1.0f);

//     reverb.combs[10] = comb(adjust_length(COMB_TUNING_L6, sample_rate), 1.0f, 1.0f);
//     reverb.combs[11] = comb(adjust_length(COMB_TUNING_R6, sample_rate), 1.0f, 1.0f);

//     reverb.combs[12] = comb(adjust_length(COMB_TUNING_L7, sample_rate), 1.0f, 1.0f);
//     reverb.combs[13] = comb(adjust_length(COMB_TUNING_R7, sample_rate), 1.0f, 1.0f);

//     reverb.combs[14] = comb(adjust_length(COMB_TUNING_L8, sample_rate), 1.0f, 1.0f);
//     reverb.combs[15] = comb(adjust_length(COMB_TUNING_R8, sample_rate), 1.0f, 1.0f);


//     reverb.allpass[0] = init_all_pass(adjust_length(ALLPASS_TUNING_L1, sample_rate), 0.5f);
//     reverb.allpass[1] = init_all_pass(adjust_length(ALLPASS_TUNING_R1, sample_rate), 0.5f);

//     reverb.allpass[2] = init_all_pass(adjust_length(ALLPASS_TUNING_L2, sample_rate), 0.5f);
//     reverb.allpass[3] = init_all_pass(adjust_length(ALLPASS_TUNING_R2, sample_rate), 0.5f);

//     reverb.allpass[4] = init_all_pass(adjust_length(ALLPASS_TUNING_L3, sample_rate), 0.5f);
//     reverb.allpass[5] = init_all_pass(adjust_length(ALLPASS_TUNING_R3, sample_rate), 0.5f);

//     reverb.allpass[6] = init_all_pass(adjust_length(ALLPASS_TUNING_L4, sample_rate), 0.5f);
//     reverb.allpass[7] = init_all_pass(adjust_length(ALLPASS_TUNING_R4, sample_rate), 0.5f);

//     reverb.dampening = dampening;
//     reverb.dry = dry;
//     reverb.gain = gain;
//     reverb.room_size = room_size;
//     reverb.wet = wet;
//     reverb.width = width;

//     return reverb;
// }

// void reverb(Reverb* r, float* input, float* output, int n, int stereo){
//     float outl = 0.0;
//     float outr = 0.0;
//     if(!stereo){
//         for(int i = 0; i < n; i++){
//             // for(int comb = 0; comb < 8; comb++){
//             //     outl += filter_comb(r->combs+(comb*2+0), input[i]);
//             //     outr += filter_comb(r->combs+(comb*2+1), input[i]);
//             // }
//             for(int allpass = 0; allpass < 4; allpass++){
//                 outl += filter_allpass(r->allpass+2*allpass+0, outl);
//                 outr += filter_allpass(r->allpass+2*allpass+1, outr);
//             }
//             output[i] = ((outl+outr)/2)*4;
//         }
//     }
// }



#include <reverb.h>


#define undenormalize(n) { if (xabs(n) < 1e-37) { (n) = 0; } }


static inline float xabs(float n) {
  return n < 0 ? -n : n;
}


static inline void zeroset(void *buf, int n) {
  while (n--) { ((char*) buf)[n] = 0; }
}


static inline float allpass_process(fv_Allpass *ap, float input) {
  float bufout = ap->buf[ap->bufidx];
  undenormalize(bufout);

  float output = -input + bufout;
  ap->buf[ap->bufidx] = input + bufout * ap->feedback;

  if (++ap->bufidx >= ap->bufsize) {
    ap->bufidx = 0;
  }

  return output;
}


static inline float comb_process(fv_Comb *cmb, float input) {
  float output = cmb->buf[cmb->bufidx];
  undenormalize(output);

  cmb->filterstore = output * cmb->damp2 + cmb->filterstore * cmb->damp1;
  undenormalize(cmb->filterstore);

  cmb->buf[cmb->bufidx] = input + cmb->filterstore * cmb->feedback;

  if (++cmb->bufidx >= cmb->bufsize) {
    cmb->bufidx = 0;
  }

  return output;
}


static inline void comb_set_damp(fv_Comb *cmb, float n) {
  cmb->damp1 = n;
  cmb->damp2 = 1.0 - n;
}


void fv_init(fv_Context *ctx) {
  zeroset(ctx, sizeof(*ctx));

  for (int i = 0; i < FV_NUMALLPASSES; i++) {
    ctx->allpassl[i].feedback = 0.5;
    ctx->allpassr[i].feedback = 0.5;
  }

  fv_set_samplerate(ctx, FV_INITIALSR);
  fv_set_wet(ctx, FV_INITIALWET);
  fv_set_roomsize(ctx, FV_INITIALROOM);
  fv_set_dry(ctx, FV_INITIALDRY);
  fv_set_damp(ctx, FV_INITIALDAMP);
  fv_set_width(ctx, FV_INITIALWIDTH);
}


void fv_mute(fv_Context *ctx) {
  for (int i = 0; i < FV_NUMCOMBS; i++) {
    zeroset(ctx->combl[i].buf, sizeof(ctx->combl[i].buf));
    zeroset(ctx->combr[i].buf, sizeof(ctx->combr[i].buf));
  }
  for (int i = 0; i < FV_NUMALLPASSES; i++) {
    zeroset(ctx->allpassl[i].buf, sizeof(ctx->allpassl[i].buf));
    zeroset(ctx->allpassr[i].buf, sizeof(ctx->allpassr[i].buf));
  }
}


static void update(fv_Context *ctx) {
  ctx->wet1 = ctx->wet * (ctx->width * 0.5 + 0.5);
  ctx->wet2 = ctx->wet * ((1 - ctx->width) * 0.5);

  if (ctx->mode >= FV_FREEZEMODE) {
    ctx->roomsize1 = 1;
    ctx->damp1 = 0;
    ctx->gain = FV_MUTED;

  } else {
    ctx->roomsize1 = ctx->roomsize;
    ctx->damp1 = ctx->damp;
    ctx->gain = FV_FIXEDGAIN;
  }

  for (int i = 0; i < FV_NUMCOMBS; i++) {
    ctx->combl[i].feedback = ctx->roomsize1;
    ctx->combr[i].feedback = ctx->roomsize1;
    comb_set_damp(&ctx->combl[i], ctx->damp1);
    comb_set_damp(&ctx->combr[i], ctx->damp1);
  }
}


void fv_set_samplerate(fv_Context *ctx, float value) {
  const int combs[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 };
  const int allpasses[] = { 556, 441, 341, 225 };

  double multiplier = value / FV_INITIALSR;

  /* init comb buffers */
  for (int i = 0; i < FV_NUMCOMBS; i++) {
    ctx->combl[i].bufsize = combs[i] * multiplier;
    ctx->combr[i].bufsize = (combs[i] + FV_STEREOSPREAD) * multiplier;
  }

  /* init allpass buffers */
  for (int i = 0; i < FV_NUMALLPASSES; i++) {
    ctx->allpassl[i].bufsize = allpasses[i] * multiplier;
    ctx->allpassr[i].bufsize = (allpasses[i] + FV_STEREOSPREAD) * multiplier;
  }
}


void fv_set_mode(fv_Context *ctx, float value) {
  ctx->mode = value;
  update(ctx);
}


void fv_set_roomsize(fv_Context *ctx, float value) {
  ctx->roomsize = value * FV_SCALEROOM + FV_OFFSETROOM;
  update(ctx);
}


void fv_set_damp(fv_Context *ctx, float value) {
  ctx->damp = value * FV_SCALEDAMP;
  update(ctx);
}


void fv_set_wet(fv_Context *ctx, float value) {
  ctx->wet = value * FV_SCALEWET;
  update(ctx);
}


void fv_set_dry(fv_Context *ctx, float value) {
  ctx->dry = value * FV_SCALEDRY;
}


void fv_set_width(fv_Context *ctx, float value) {
  ctx->width = value;
  update(ctx);
}


void fv_process(fv_Context *ctx, float *buf, int n) {
  for (int i = 0; i < n; i += 2) {
    float outl = 0;
    float outr = 0;
    float input = (buf[i] + buf[i + 1]) * ctx->gain;

    /* accumulate comb filters in parallel */
    for (int i = 0; i < FV_NUMCOMBS; i++) {
      outl += comb_process(&ctx->combl[i], input);
      outr += comb_process(&ctx->combr[i], input);
    }

    /* feed through allpasses in series */
    for (int i = 0; i < FV_NUMALLPASSES; i++) {
      outl = allpass_process(&ctx->allpassl[i], outl);
      outr = allpass_process(&ctx->allpassr[i], outr);
    }

    /* replace buffer with output */
    buf[i  ] = outl * ctx->wet1 + outr * ctx->wet2 + buf[i  ] * ctx->dry;
    buf[i+1] = outr * ctx->wet1 + outl * ctx->wet2 + buf[i+1] * ctx->dry;
  }
}
