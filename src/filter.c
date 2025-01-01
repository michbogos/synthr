#include<defs.h>
#include<math.h>
#include<stdlib.h>
#include<filter.h>

Biquad biquad(enum BiquadType type){
    Biquad b;
    b.in1 = 0;
    b.in2 = 0;
    b.in3 = 0;
    b.out1 = 0;
    b.out2 = 0;
    b.out3 = 0;
    b.type = type;
    return b;
}

CombFilter comb(int n, float alpha, float dampening){
  CombFilter cf;
  cf.buffer = init_circular_buffer(sizeof(float), n);
  float zero = 0;
  for(int i = 0; i < n; i++){
    write_circular_buffer(&cf.buffer, &zero, 1);
  }
  cf.alpha = alpha;
  cf.n = n;
  cf.filter_state = 0.0f;
  cf.dampening = dampening;
  return cf;
}

AllpassFilter init_all_pass(int n, float feedback){
  AllpassFilter apf;
  apf.buffer = init_circular_buffer(sizeof(float), n);
  float zero = 0;
  for(int i = 0; i < n; i++){
    write_circular_buffer(&apf.buffer, &zero, 1);
  }
  apf.feedback = feedback;
  apf.n = n;
  return apf;
}

float filter_comb(CombFilter* filter, float sample){
  float filter_val = 0;
  read_circular_buffer(&filter->buffer, &filter_val, 1);
  filter->filter_state = (filter_val*(1-filter->dampening)+filter->filter_state*filter->dampening);
  float output = sample+filter->filter_state*filter->alpha;
  write_circular_buffer(&filter->buffer, &sample, 1);
  return filter_val;
}

float filter_allpass(AllpassFilter* filter, float sample){
  float filter_val = 0;
  read_circular_buffer(&filter->buffer, &filter_val, 1);
  float res = filter_val-sample;
  float delay = sample*filter->feedback;
  write_circular_buffer(&filter->buffer, &delay, 1);
  return res;
}

Filter filter_coeffs(float* a, float* b, int numa, int numb){
    Filter f;
    f.a = malloc(numa*sizeof(float));
    for(int i = 0; i < numa; i++){
        f.a[i] = a[i];
    }
    f.b = malloc(numb*sizeof(float));
    for(int i = 0; i < numb; i++){
        f.b[i] = b[i];
    }
    f.in = malloc(numb*sizeof(float));
    f.out = malloc(numa*sizeof(float));
    f.numa = numa;
    f.numb = numb;
    return f;
};

float filter(float input, float fs, Biquad* filter, float fc, float Q, float dBgain)
{
  float w0 = 2*PI*fc/fs;
  float cosw0 = cosf(w0);
  float sinw0 = sinf(w0);
  float alpha = sinw0/(2*Q);
  float A = powf(10,(dBgain/40));
  float a0 = 0.0;
  float a1 = 0.0;
  float a2 = 0.0;
  float b0 = 0.0;
  float b1 = 0.0;
  float b2 = 0.0;

  switch (filter->type){
    case LOWPASS:
      b0 =  (1.0f - cosw0)/2.0f;
      b1 =   1.0f - cosw0;
      b2 =  (1.0f - cosw0)/2.0f;
      a0 =   1.0f + alpha;
      a1 =  -2.0f * cosw0;
      a2 =   1.0f - alpha;
      break;
    case HIGHPASS:
      b0 =  (1 + cosw0)/2;
      b1 = -(1 + cosw0);
      b2 =  (1 + cosw0)/2;
      a0 =   1 + alpha;
      a1 =  -2*cosw0;
      a2 =   1 - alpha;
      break;
    case BANDPASSQ:
      b0 =   Q*alpha;
      b1 =   0;
      b2 =   -Q*alpha;
      a0 =   1 + alpha;
      a1 =  -2*cosw0;
      a2 =   1 - alpha;
      break;
    case BANDPASS0:
      b0 =   alpha;
      b1 =   0;
      b2 =  -alpha;
      a0 =   1 + alpha;
      a1 =  -2*cosw0;
      a2 =   1 - alpha;
      break;
    case NOTCH:
      b0 =   1;
      b1 =  -2*cosw0;
      b2 =   1;
      a0 =   1 + alpha;
      a1 =  -2*cosw0;
      a2 =   1 - alpha;
      break;
    case APF:
      b0 =   1 - alpha;
      b1 =  -2*cosw0;
      b2 =   1 + alpha;
      a0 =   1 + alpha;
      a1 =  -2*cosw0;
      a2 =   1 - alpha;
    case PEAKEQ:
      b0 =   1 + alpha*A;
      b1 =  -2*cosw0;
      b2 =   1 - alpha*A;
      a0 =   1 + alpha/A;
      a1 =  -2*cosw0;
      a2 =   1 - alpha/A;
      break;
    case HIGHSHELF:
      b0 = A*((A+1)-(A-1)*cosw0 + 2*sqrtf(A)*alpha);
      b1 = 2*A*((A-1)-(A+1)*cosw0);
      b2 = A*((A+1)-(A-1)*cosw0 - 2*sqrtf(A)*alpha);
      a0 = (A+1)+(A-1)*cosw0 + 2*sqrtf(A)*alpha;
      a1 = -2*((A-1)+(A+1)*cosw0);
      a2 = (A+1) + (A-1)*cosw0 - 2*sqrtf(A)*alpha;
      break;
    case LOWSHELF:
      b0 = A*((A+1) + (A-1)*cosw0 + 2*sqrtf(A)*alpha);
      b1 = -2*A*((A-1) + (A+1)*cosw0);
      b2 = A*((A+1) + (A-1)*cosw0 - 2*sqrtf(A)*alpha);
      a0 = (A+1) - (A-1)*cosw0 + 2*sqrtf(A)*alpha;
      a1 = 2*((A-1) - (A+1)*cosw0);
      a2 = (A+1) - (A-1)*cosw0 - 2*sqrt(A)*alpha;
      break;
  }

  filter->in1 = filter->in2;
  filter->in2 = filter->in3;
  filter->in3 = input;

  filter->out1 = filter->out2;
  filter->out2 = filter->out3;

  filter->out3 = (b0/a0)*filter->in3 + (b1/a0)*filter->in2 + (b2/a0)*filter->in1 - (a1/a0)*filter->out2 - (a2/a0)*filter->out1;
  return filter->out3;
}