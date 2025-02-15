#ifndef EFFECTS_H
#define EFFECTS_H

#include<math.h>

float distortion_foldback(float in, float threshold){
  while (in > threshold || in < -threshold)
  {
    in = fabsf(fabsf(fmodf(in - threshold, threshold*4)) - threshold*2) - threshold;
  }
  return in;
}

float distortion_exp(float x){
  return x/fabsf(x)*(1.0f-exp(-powf(x,2)/fabsf(x)));
}

float bit_crusher(float x, float bits){
  return ((int)(x*powf(2, bits)))/powf(2, bits);
}

#endif