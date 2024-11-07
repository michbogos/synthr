#ifndef WAV_H
#define WAV_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct wav_header
{
  char riff[4];                  /* "RIFF"                                  */
  unsigned int flength;          /* file length in bytes                    */
  char wave[4];                  /* "WAVE"                                  */
  char fmt[4];                   /* "fmt "                                  */
  unsigned int chunk_size;       /* size of FMT chunk in bytes (usually 16) */
  unsigned short format_tag;     /* 1=PCM, 257=Mu-Law, 258=A-Law, 259=ADPCM */
  unsigned short num_chans;      /* 1=mono, 2=stereo                        */
  unsigned int srate;            /* Sampling rate in samples per second     */
  unsigned int bytes_per_sec;    /* bytes per second = srate*bytes_per_samp */
  unsigned short bytes_per_samp; /* 2=16-bit mono, 4=16-bit stereo          */
  unsigned short bits_per_samp;  /* Number of bits per sample               */
  char data[4];                  /* "data"                                  */
  unsigned int dlength;          /* data length in bytes (filelength - 44)  */
};

void write_wav(float* buffer, unsigned int length, unsigned int sample_rate, unsigned int num_channels, char* filename){
    struct wav_header header = {};
    memcpy(header.riff, "RIFF", 4);
    memcpy(header.wave, "WAVE", 4);
    memcpy(header.fmt, "fmt ", 4);
    memcpy(header.data, "data", 4);
    header.chunk_size = 16;
    header.format_tag = 1;
    header.bits_per_samp = 16*sample_rate;
    header.srate = sample_rate;
    header.num_chans = num_channels;
    header.bytes_per_samp = header.bits_per_samp/8*header.num_chans;
    header.bytes_per_sec = header.srate*header.bytes_per_samp;
    header.dlength = length*header.bytes_per_samp;
    header.flength = header.dlength+sizeof(struct wav_header);

    short* out_buffer = (short*)malloc(length*header.bytes_per_samp);

    for(int i = 0; i < length; i++){
        //Clip to -1.0--1.0
        float sample = buffer[i] > 1.0f ? 1.0f : buffer[i];
        sample = sample < -1.0f ? -1.0f : sample;
        out_buffer[i] = (short)(sample*__INT16_MAX__);
    }

    FILE *fp = fopen(filename, "w");
    fwrite(&header, 1, sizeof(struct wav_header), fp);
    fwrite(buffer, 2, length, fp);
}

#endif