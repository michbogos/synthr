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
  *phase += frequency/(1.0/dt);
  if(*phase > 1.0){
    *phase -= 1.0;
  }
  float phase2 = *phase+0.5;
  if(phase2 > 1.0){
    phase2 -= 1.0;
  }
  if(*phase<0.5){
    return 1 + poly_blep(*phase, frequency/(1.0/dt))-poly_blep(phase2, frequency/(1.0f/dt));
  }
  else{
    return -1+ poly_blep(*phase, frequency/(1.0/dt))-poly_blep(phase2, frequency/(1.0f/dt));
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
    *phase += frequency/(1.0/dt);
    if(*phase > 1.0){
      *phase -= 1.0;
    }
    return 2*(*phase)-1.0f - poly_blep(*phase, frequency/(1.0/dt));
}


float osc_tri(float frequency, float* phase, float dt){
    *phase += frequency/(1.0/dt);
    if(*phase > 1.0f){
      *phase -= 1.0f;
    } 
    // float t1 = *phase+0.25;
    // if(t1 > 1.0f){
    //   t1 -= 1.0f;
    // } 
    // float t2 = *phase+0.75;
    // if(t2 > 1.0f){
    //   t2 -= 1.0f;
    // } 
    float y = *phase * 4;

    if (y >= 3) {
        y -= 4;
    } else if (y > 1) {
        y = 2 - y;
    }
    
    return y+4*frequency/(1.0/dt);//-poly_blep(t1, frequency/(1.0/dt))+poly_blep(t2, frequency/(1.0/dt));
}


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