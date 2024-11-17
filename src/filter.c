#include<filter.h>

float filter_lowpass(float input, struct filterData* data, double fc, double res)
{
  double f = fc * 1.16;
  double fb = res * (1.0 - 0.15 * f * f);
  input -= data->out4 * fb;
  input *= 0.35013 * (f*f)*(f*f);
  data->out1 = input + 0.3 * data->in1 + (1 - f) * data->out1; // Pole 1
  data->in1  = input;
  data->out2 = data->out1 + 0.3 * data->in2 + (1 - f) * data->out2;  // Pole 2
  data->in2  = data->out1;
  data->out3 = data->out2 + 0.3 * data->in3 + (1 - f) * data->out3;  // Pole 3
  data->in3  = data->out2;
  data->out4 = data->out3 + 0.3 * data->in4 + (1 - f) * data->out4;  // Pole 4
  data->in4  = data->out3;
  return data->out4;
}