#include <soundio/soundio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <oscillators.h>
#include <wavetable.h>
#include <wavefile.h>
#include <wavegraph.h>
#include <defs.h>
#include <filter.h>
#include <formant.h>

float frequency = 200.0f;
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
Biquad lpf;
Biquad hpf;
Formant form;
float time = 0.0f;

void underflow_callback(){
    printf("Underflowing framesn\n");
}

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
        getNodeOutput(mul, frame_count, samples, 1.0f/float_sample_rate);

        for(int i = 0; i < frame_count; i++){
            // samples[i] = filter(samples[i], float_sample_rate, &lpf, 1800, 20, 5);
            // samples[i] = filter(samples[i], float_sample_rate, &hpf, 800, 20, 5);
            // samples[i] = formantize(float_sample_rate, samples[i], form);
        }

        for (int frame = 0; frame < frame_count; frame += 1) {
            // float sample = 0.5;
            // for(int k = 1; k <32; k++){
            //     sample -= k%2 ? (1.0/3.1415926)*(1.0/k)*sin(2*3.1415926*k*((float)global_frame/float_sample_rate)*440) : (1.0/3.1415926)*(-1.0/k)*sin(2*3.1415926*k*((float)global_frame/float_sample_rate)*440);
            // }
            // float sample = osc_tbl(frequency, &phase, 1.0f/float_sample_rate, &sawtable)*0.7;
            //float sample = osc_saw(global_frame, float_sample_rate,  exp2(global_frame/float_sample_rate));
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
    tritable = wtbl_saw(48000, 4096, 20);
    sawtable = wtbl_saw(48000, 4096, 20);
    p = nodeNumber(0.0f);
    p2 = nodeNumber(0.0f);
    f2 = nodeNumber(0.03f);
    f = nodeNumber(400.0f);
    osc1 = nodeWavetable(f, p, &sawtable);
    s = nodeSin(f2, p2);
    WaveNode add = nodeAdd(nodeDiv(s, nodeNumber(2.0f)), nodeNumber(0.5f));
    mul = nodeMul(osc1, add);
    lpf = biquad(LOWSHELF);
    hpf = biquad(HIGHSHELF);
    form = make_formant(FORMANT_TABLE[0]);


    float samples[48000*5] = {};
    getNodeOutput(mul, 5*48000, samples, 1.0/48000.0);
    write_wav(samples, 48000*5, 48000, 1, "test.wav");
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
    outstream->underflow_callback = underflow_callback;

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