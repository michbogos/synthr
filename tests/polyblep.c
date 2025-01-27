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

int main(){
    float samples[5*48000];
    float frequency = 80.0f;
    float phase = 0;
    for(int i = 0; i < 5*48000; i++){
        samples[i] = osc_saw(frequency, &phase, 1.0/48000.0);
        if(i%1000 == 0){
            frequency *= 1.03;
        }
    }
    write_wav(samples, 5*48000, 48000.0f, 1, "test.wav");
    return 0;
}