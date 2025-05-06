// // TODO: Add three timers: global, group and per-note timer. For modulation purpouses
// #include <soundio/soundio.h>
// #define __LINUX_ALSA__
// #include <rtmidi_c.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>
// #include <oscillators.h>
// #include <wavetable.h>
// #include <wavefile.h>
// #include <wavegraph.h>
// #include <defs.h>
// #include <formant.h>
// #include <envelope.h>
// #include <delay.h>
// #include <reverb.h>
// #include <midi.h>
// #include <fft.h>
// #include <cJSON.h>

// WaveNode nodes[32];
// WaveNode channels[NUM_VOICES][32];

// float frequency = 200.0f;
// float phase = 0.0f;
// Delay d;
// Wavetable sawtable;
// Wavetable tritable;
// WaveNode p;
// WaveNode f;
// WaveNode f2;
// WaveNode p2;
// WaveNode osc1;
// WaveNode osc2;
// WaveNode add;
// WaveNode mul;
// WaveNode s;
// WaveNode midi_node;
// Biquad lpf;
// Biquad hpf;
// Formant form;
// float time = 0.0f;
// WaveNode noise;
// CombFilter cmb;
// CombFilter cmb2;
// ADSREnvelope adsr;
// circular_buffer cbuf;
// // Reverb verb;
// MidiState midi_state;

// void underflow_callback(){
//     printf("Underflowing framesn\n");
// }

// static float seconds_offset = 0.0f;
// static void write_callback(struct SoundIoOutStream *outstream,
//         int frame_count_min, int frame_count_max)
// {
//     const struct SoundIoChannelLayout *layout = &outstream->layout;
//     float float_sample_rate = outstream->sample_rate;
//     float seconds_per_frame = 1.0f / float_sample_rate;
//     struct SoundIoChannelArea *areas;
//     int frames_left = frame_count_max;
//     int err;

//     while (frames_left > 0) {
//         int frame_count = frames_left;

//         //printf("%f, %d\n", ((ADSREnvelope*)channels[0][5].value)->res,((ADSREnvelope*)channels[0][5].value)->state);

//         if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
//             fprintf(stderr, "%s\n", soundio_strerror(err));
//             exit(1);
//         }

//         if (!frame_count)
//             break;
        
//         float samples[frame_count];
//         for(int i = 0; i < frame_count; i++){
//             samples[i] = 0;
//         }
//         float tmp[frame_count];
//         for(int voice = 0; voice < NUM_VOICES; voice++){
//             getNodeOutput(2, channels[voice], 32, frame_count, tmp, 1.0f/float_sample_rate);
//             for(int i = 0; i < frame_count; i++){
//                 samples[i] += tmp[i];
//             }
//         }
        

//         // time += frame_count/float_sample_rate;
//         // if(((int)(time/1.0f))%2 == adsr.key_pressed){
//         //     //printf("%f\n", time);
//         //     adsr.key_pressed = 1-adsr.key_pressed;
//         // }
//         // char key_pressed = 0;
//         // for(int i = 0; i < NUM_VOICES; i++){
//         //     if(midi_state.notes[i]>0){
//         //         key_pressed = 1;
//         //         break;
//         //     }
//         // }
//         // for(int i = NUM_VOICES; i >= 0; i--){
//         //     adsr.key_pressed = 0;
//         //     if(midi_state.notes[i] > 0){
//         //         frequency = 261.625565*powf(powf(2.0f, 1.0f/12.0f), midi_state.notes[i]-60.0f+12.0f*((float)((int)midi_state.pitch_bend-16384))/16384.0f);
//         //         adsr.key_pressed = 1;
//         //         break;
//         //     }
//         // }
//         // float adsr_vals[frame_count];
//         // gen_adsr_envelope(&adsr, adsr_vals, frame_count, float_sample_rate);
//         // //getNodeOutput(noise, frame_count, samples, 1.0f/float_sample_rate);

//         //     for(int i = 0; i < frame_count; i++){
//         //         samples[i] = osc_tbl(frequency, &phase, 1/float_sample_rate, &sawtable);
//         //         samples[i] = filter(samples[i], float_sample_rate, &lpf, 1800, 20, 5);
//         //         samples[i] = filter(samples[i], float_sample_rate, &lpf, 3200, 1, 0);
//         //         samples[i] = filter_comb(&cmb, samples[i]);
//         //         samples[i] = filter_comb(&cmb2, samples[i]);
//         //         samples[i] = filter(samples[i], float_sample_rate, &hpf, 3200, 1, 0);
//         //         // samples[i] = formantize(float_sample_rate, samples[i], form);
//         //     }

//         // write_circular_buffer(&cbuf, samples, frame_count);

//         // float output[frame_count];

//         // read_circularbuffer(&cbuf, output, frame_count);

//         // float output[frame_count];

//         // for(int i = 0; i < frame_count; i++){
//         //     samples[i] = samples[i]*adsr_vals[i];
//         //     //samples[i] = formantize(float_sample_rate, samples[i], form);
//         // }

//         // float del[frame_count];

//         // delay(samples, del, &d, frame_count);

//         // for(int i =0; i < frame_count; i++){
//         //     samples[i] += del[i];
//         // }

//         //reverb(&verb, samples, samples, frame_count, 0);

//         for (int frame = 0; frame < frame_count; frame += 1) {
//             // float sample = 0.5;
//             // for(int k = 1; k <32; k++){
//             //     sample -= k%2 ? (1.0/3.1415926)*(1.0/k)*sin(2*3.1415926*k*((float)global_frame/float_sample_rate)*440) : (1.0/3.1415926)*(-1.0/k)*sin(2*3.1415926*k*((float)global_frame/float_sample_rate)*440);
//             // }
//             // float sample = osc_tbl(frequency, &phase, 1.0f/float_sample_rate, &sawtable)*0.7;
//             //float sample = osc_saw(global_frame, float_sample_rate,  exp2(global_frame/float_sample_rate));
//             //printf("%f\n", exp2(global_frame/float_sample_rate));
//             for (int channel = 0; channel < layout->channel_count; channel += 1) {
//                 float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
//                 *ptr = samples[frame];
//             }
//         }

//         cmb.alpha = 2*sinf(time);
//         cmb2.alpha = 1.5*sinf(-2*time+1.0);

//         if ((err = soundio_outstream_end_write(outstream))) {
//             fprintf(stderr, "%s\n", soundio_strerror(err));
//             exit(1);
//         }

//         frames_left -= frame_count;
//     }
// }

// int main(int argc, char **argv) {
//     for(int i = 0; i < NUM_VOICES; i++){
//         midi_state.velocities[i] = 0;
//     }
//     int numPorts = -1;
//     char buf[255];
//     RtMidiInPtr midiin = rtmidi_in_create (RTMIDI_API_LINUX_ALSA, "Synthr", 1024);
//     printf("Created MIDI device: %s\n", buf);
//     numPorts = rtmidi_get_port_count(midiin);
//     printf("Num Ports: %d", numPorts);
//     rtmidi_open_port (midiin, numPorts>1?1:0, buf);
//     printf("Opened port: %s\n", buf);

//     rtmidi_in_set_callback(midiin, midi_callback, &midi_state);

//     float val = -1.0f;
//     d = init_delay(24000, 0.3);
//     //verb = init_reverb(0.5, 0.5, 1.0, 0.5, 0.5, 0.9, 48000);

//     // for(int i = 0; i < 24000; i++){
//     //     write_circular_buffer(&cbuf, &val, 1);
//     // }
//     tritable = wtbl_sqr(48000, 4096, 20);
//     sawtable = wtbl_sqr(48000, 4096, 20);
//     adsr.ab = 3.0f/48000.0f;
//     adsr.ac = 1.0f;
//     adsr.db = -1.0f/48000.0f;
//     adsr.dc = 1.0f;
//     adsr.sustain = 0.5f;
//     adsr.rb = -1.0f/48000.0f;
//     adsr.rc = 1.00f;
//     adsr.state = 0;
//     nodes[0] = nodeNumber(1600.0f);
//     nodes[1] = nodeFilterLowpass(4, 0, 9);
//     nodes[2] = nodeMul(22, 21);
//     nodes[3] = nodeMidiGate(0, &midi_state);
//     nodes[4] = nodePolygon(20, 17);
//     nodes[5] = nodeAdsr(3, &adsr);
//     nodes[6] = nodeNumber(4.0f);
//     nodes[7] = nodeVelocity(0, &midi_state);
//     nodes[8] = nodeMul(7, 6);
//     nodes[9] = nodeAdd(8, 10);
//     nodes[10] = nodeNumber(0.001);
//     nodes[11] = nodeModWheel(&midi_state);
//     nodes[12] = nodeSin(18);
//     nodes[13] = nodeAdd(12, 14);
//     nodes[14] = nodeNumber(3.0f);
//     nodes[15] = nodeMul(11, 16);
//     nodes[16] = nodeNumber(3.0f);
//     nodes[17] = nodeAdd(15, 13);
//     nodes[18] = nodeMul(11, 19);
//     nodes[19] = nodeNumber(15);
//     nodes[20] = nodeMidiPitch(0, &midi_state);
//     nodes[21] = nodeMidiControl(96, &midi_state);
//     nodes[22] = nodeMul(1, 5);
//     for(int i = 0; i < NUM_VOICES; i++){
//         for(int j = 0; j < 32; j++){
//             channels[i][j] = copyNode(nodes[j]);
//             if(channels[i][j].type==MIDI_GATE || channels[i][j].type==VELOCITY || channels[i][j].type==MIDI_PITCH){
//                 *(int*)(((char*)channels[i][j].value)+sizeof(MidiState*)) = i;
//             }
//         }
//     }
//     // WaveNode add = nodeAdd(nodeDiv(s, nodeNumber(2.0f)), nodeNumber(0.5f));
//     // mul = nodeMul(osc1, add);
//     lpf = biquad(LOWPASS);
//     hpf = biquad(LOWPASS);
//     form = make_formant(FORMANT_TABLE[0]);
//     noise = nodeBrownNoise();
//     cmb = comb(16, 1.0f, 1.0f);
//     cmb2 = comb(8, 1.0f, 1.0f);


//     // float samples[48000*5] = {};
//     // getNodeOutput(mul, 5*48000, samples, 1.0/48000.0);
//     // write_wav(samples, 48000*5, 48000, 1, "test.wav");
//     int err;
//     struct SoundIo *soundio = soundio_create();
//     if (!soundio) {
//         fprintf(stderr, "out of memory\n");
//         return 1;
//     }

//     if ((err = soundio_connect(soundio))) {
//         fprintf(stderr, "error connecting: %s", soundio_strerror(err));
//         return 1;
//     }

//     soundio_flush_events(soundio);

//     int default_out_device_index = soundio_default_output_device_index(soundio);
//     printf("%d\n", soundio_output_device_count(soundio));
//     if (default_out_device_index < 0) {
//         fprintf(stderr, "no output device found");
//         return 1;
//     }

//     struct SoundIoDevice *device = soundio_get_output_device(soundio, default_out_device_index);
//     if (!device) {
//         fprintf(stderr, "out of memory");
//         return 1;
//     }

//     fprintf(stderr, "Output device: %s\n", device->name);

//     struct SoundIoOutStream *outstream = soundio_outstream_create(device);
//     outstream->software_latency = 0.05;
//     outstream->format = SoundIoFormatFloat32NE;
//     outstream->write_callback = write_callback;
//     outstream->underflow_callback = underflow_callback;

//     if ((err = soundio_outstream_open(outstream))) {
//         fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
//         return 1;
//     }

//     if (outstream->layout_error)
//         fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

//     if ((err = soundio_outstream_start(outstream))) {
//         fprintf(stderr, "unable to start device: %s", soundio_strerror(err));
//         return 1;
//     }

//     for (;;)
//         soundio_wait_events(soundio);

//     soundio_outstream_destroy(outstream);
//     soundio_device_unref(device);
//     soundio_destroy(soundio);
//     rtmidi_close_port(midiin);
//     rtmidi_in_free(midiin);
//     return 0;
// }

#include<serialize.h>
#include<wavegraph.h>
#include<oscillators.h>
#include<filter.h>
#include<stdio.h>
#include<wavefile.h>

int main(){
    WaveNode wavenodes[4];
    float buffer[48000];
    float buffer2[48000];
    float phase = 0;
    for(int i = 0; i < 48000; i++){
        buffer[i] = osc_saw(440, &phase, 1.0f/48000.0f);
    }

    for(int i = 0; i < 48000; i++){
        buffer2[i] = osc_saw(110, &phase, 1.0f/48000.0f);
    }

    write_wav(buffer2, 48000, 48000, 1, "normal.wav");

    Biquad peak1 = biquad(PEAKEQ);
    Biquad peak2 = biquad(PEAKEQ);

    for(int i = 0; i < 48000; i++){
        buffer[i] = filter(buffer[i], 48000, &peak1, 850, 3.0, 24.0f);
    }

    for(int i = 0; i < 48000; i++){
        buffer[i] = filter(buffer[i], 48000, &peak2, 1610, 3.0, 8.0f);
    }

    for(int i = 0; i < 48000; i++){
        buffer2[i] = filter(buffer2[i], 48000, &peak1, 250, 3, 24.0f);
    }

    for(int i = 0; i < 48000; i++){
        buffer2[i] = filter(buffer2[i], 48000, &peak2, 595, 3, 6.0f);
    }

    write_wav(buffer, 48000, 48000, 1, "a.wav");
    write_wav(buffer2, 48000, 48000, 1, "u.wav");

    //write_wav(buffer, 48000, 48000, 1, "formant.wav");
    wavenodes[0] = nodeNumber(1);
    wavenodes[1] = nodeNumber(1);
    wavenodes[2] = nodeAdd(0,1);
    wavenodes[3] = nodeTri(2);

    char* res = export_nodegraph(wavenodes, 4);

    printf("%s\n", res);

    WaveNode* imported = import_nodegraph(res);
    char* reexport = export_nodegraph(imported, 4);

    printf("%s\n", reexport);
    return 0;
}