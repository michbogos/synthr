#include"wavefile.h"
#include<math.h>

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

float osc_saw(float frequency, float* phase, float dt){
    *phase += frequency/(1.0/dt);
    if(*phase > 1.0){
      *phase -= 1.0;
    }
    return 2*(*phase)-1.0f - poly_blep(*phase, frequency/(1.0/dt));
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

int main(){
    float samples[10*48000];
    float frequency = 80.0f;
    float phase = 0;
    for(int i = 0; i < 10*48000; i++){
        samples[i] = osc_tri(frequency, &phase, 1.0/48000.0);
        if(i%1000 == 0){
            frequency *= 1.01;
        }
    }
    write_wav(samples, 10*48000, 48000.0f, 1, "test.wav");
    return 0;
}