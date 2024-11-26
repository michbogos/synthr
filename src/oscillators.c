#include<oscillators.h>
#include<wavetable.h>
#include<math.h>
#include<defs.h>

double poly_blep(float t, float dt)
{
  // 0 <= t < 1
  if (t < dt)
  {
    t /= dt;
    // 2 * (t - t^2/2 - 0.5)
    return t+t - t*t - 1.;
  }

  // -1 < t < 0
  else if (t > 1. - dt)
  {
    t = (t - 1.) / dt;
    // 2 * (t^2/2 + t + 0.5)
    return t*t + t+t + 1.;
  }

  // 0 otherwise
  else
  {
    return 0.;
  }
}

float osc_sqr(float frequency, float* phase, float dt){
  *phase += dt*frequency;
  if(*phase > 1.0){
    *phase -= 1.0;
  }
  if(*phase-floorf(*phase) > 0.5/(frequency)){
    return 1;
  }
  else{
    return -1;
  }
}

float osc_sin(float frequency, float* phase, float dt){
    *phase += dt*frequency;
    if(*phase > 1.0){
      *phase -= 1.0;
    }
    return sin(2*PI*(*phase));
}

float osc_saw(float frequency, float* phase, float dt){
    *phase += dt*frequency;
    if(*phase > 1.0){
      *phase -= 1.0;
    }
    return ((*phase*(frequency)*-floorf(*phase*(frequency)))-0.5)*2;
}


//Triangle wave phase shifts due to float to int rounding
// float osc_tri(float* frequency, float* pahse, float dt){
//     int val = (n % (int)((float)sample_rate/frequency)) - ((int)((float)sample_rate/frequency))/2.0f;
//     val = val > 0 ? val : -val;
//     return val/(float)(sample_rate/frequency)-0.5;
// }

float osc_tbl(float frequency, float* phase, float dt, Wavetable* wavetable){
    *phase += dt*(frequency);
    if(*phase > 1.0f){
      *phase -= 1.0f;
    }
    int table_idx = (int)ceil(log2(frequency/wavetable->base)) < wavetable->num_tables ? (int)ceil(log2(frequency/wavetable->base)): wavetable->num_tables-1;
    table_idx = table_idx < 0 ? 0 : table_idx;
    float* table = wavetable->tables[table_idx];
    //float idx = *phase*(frequency) - floor(*phase*(frequency));
    float subidx = *phase*wavetable->len;
    float t = subidx-floor(subidx);
    float s1 = table[(int)floor(subidx)];
    float s2 = table[(int)ceil(subidx)];
    // float t = ((float)n/sample_rate)frequency-floor(((float)n/sample_rate)frequency);
    return s1+(s2-s1)*t;
}

float osc_tbl_chorus(float frequency, float cents, int n, float* phase, float dt, Wavetable* wavetable){
    *phase += dt*(frequency);
    if(*phase > 1.0f){
      *phase -= 1.0f;
    }
    float res = 0.0f;
    for(int wave = 0; wave < n; wave++){
      float detune = cents*n;
      int table_idx = (int)ceil(log2((frequency*(powf(2, detune/1200.0f)))/wavetable->base)) < wavetable->num_tables ? (int)ceil(log2(frequency*(powf(2, detune/1200.0f))/wavetable->base)): wavetable->num_tables-1;
      table_idx = table_idx < 0 ? 0 : table_idx;
      float* table = wavetable->tables[table_idx];
      //float idx = *phase*(frequency) - floor(*phase*(frequency));
      float subidx = (*(phase)/(powf(2, detune/1200.0f)))*wavetable->len;
      float t = subidx-floor(subidx);
      float s1 = table[(int)floor(subidx)];
      float s2 = table[(int)ceil(subidx)];
      res += s1+(s2-s1)*t;
    }
    // float t = ((float)n/sample_rate)frequency-floor(((float)n/sample_rate)frequency);
    return res;
}