#include <soundio/soundio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <oscillators.h>
#include <wavetable.h>
#include <wavefile.h>
#include <wavegraph.h>

float frequency = 440.0f;
float phase = 0.0f;
Wavetable sawtable;
Wavetable tritable;
WaveNode p;
WaveNode f;
WaveNode f2;
WaveNode p2;
WaveNode osc1;
WaveNode osc2;
WaveNode add;
WaveNode mul;
WaveNode s;

static const float PI = 3.1415926535f;
static float seconds_offset = 0.0f;
static void write_callback(struct SoundIoOutStream *outstream,
        int frame_count_min, int frame_count_max)
{
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    float float_sample_rate = outstream->sample_rate;
    float seconds_per_frame = 1.0f / float_sample_rate;
    struct SoundIoChannelArea *areas;
    int frames_left = frame_count_max;
    int err;

    while (frames_left > 0) {
        int frame_count = frames_left;

        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
            fprintf(stderr, "%s\n", soundio_strerror(err));
            exit(1);
        }

        if (!frame_count)
            break;
        
        float samples[frame_count];
        //getNodeOutput(s, frame_count, samples, 1.0f/float_sample_rate);

        for (int frame = 0; frame < frame_count; frame += 1) {
            osc_sin(440, &phase, 1/float_sample_rate);
            // float sample = 0.5;
            // for(int k = 1; k <32; k++){
            //     sample -= k%2 ? (1.0/3.1415926)*(1.0/k)*sin(2*3.1415926*k*((float)global_frame/float_sample_rate)*440) : (1.0/3.1415926)*(-1.0/k)*sin(2*3.1415926*k*((float)global_frame/float_sample_rate)*440);
            // }
            // float sample = osc_tbl(frequency, &phase, 1.0f/float_sample_rate, &sawtable)*0.7;
            //float sample = osc_saw(global_frame, float_sample_rate,  exp2(global_frame/float_sample_rate));
            if((int)frequency%100 == 0){
                printf("%f\n", frequency);
            }
            //printf("%f\n", exp2(global_frame/float_sample_rate));
            for (int channel = 0; channel < layout->channel_count; channel += 1) {
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                *ptr = samples[frame];
            }
        }

        if ((err = soundio_outstream_end_write(outstream))) {
            fprintf(stderr, "%s\n", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
    }
}

int main(int argc, char **argv) {
    // float samples[48000*5] = {};
    // for(int i = 0; i < 48000*5; i++){
    //     samples[i] = 0.5*sin((i*2*3.1415926*440)/44100.0f);
    // }
    // write_wav(samples, 48000*5, 48000, 1, "test.wav");
    tritable = wtbl_sqr(48000, 4096, 20);
    sawtable = wtbl_saw(48000, 4096, 20);
    p = nodeNumber(0.0f);
    p2 = nodeNumber(0.0f);
    f2 = nodeNumber(440.0f);
    f = nodeNumber(440.0f);
    osc1 = nodeWavetable(f, p, &sawtable);
    s = nodeSin(f2, p2);
    WaveNode add = nodeAdd(s, nodeNumber(0.5f));
    mul = nodeMul(osc1, add);
    int err;
    struct SoundIo *soundio = soundio_create();
    if (!soundio) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    if ((err = soundio_connect(soundio))) {
        fprintf(stderr, "error connecting: %s", soundio_strerror(err));
        return 1;
    }

    soundio_flush_events(soundio);

    int default_out_device_index = soundio_default_output_device_index(soundio);
    printf("%d\n", soundio_output_device_count(soundio));
    if (default_out_device_index < 0) {
        fprintf(stderr, "no output device found");
        return 1;
    }

    struct SoundIoDevice *device = soundio_get_output_device(soundio, default_out_device_index);
    if (!device) {
        fprintf(stderr, "out of memory");
        return 1;
    }

    fprintf(stderr, "Output device: %s\n", device->name);

    struct SoundIoOutStream *outstream = soundio_outstream_create(device);
    outstream->format = SoundIoFormatFloat32NE;
    outstream->write_callback = write_callback;

    if ((err = soundio_outstream_open(outstream))) {
        fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
        return 1;
    }

    if (outstream->layout_error)
        fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

    if ((err = soundio_outstream_start(outstream))) {
        fprintf(stderr, "unable to start device: %s", soundio_strerror(err));
        return 1;
    }

    for (;;)
        soundio_wait_events(soundio);

    soundio_outstream_destroy(outstream);
    soundio_device_unref(device);
    soundio_destroy(soundio);
    return 0;
}