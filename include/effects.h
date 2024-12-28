#ifndef EFFECTS_H
#define EFFECTS_H

#include<math.h>

float distortion_foldback(float in, float threshold)
{
  while (in > threshold || in < -threshold)
  {
    in = fabsf(fabsf(fmodf(in - threshold, threshold*4)) - threshold*2) - threshold;
  }
  return in;
}

#endif