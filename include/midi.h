#ifndef MIDI_H
#define MIDI_H

#define NUM_VOICES 16

#include <stddef.h>


typedef struct {
    //Keep MIDI value as well as wether turned on or not
    unsigned char notes[NUM_VOICES];
    unsigned char velocities[NUM_VOICES];
    unsigned char is_on[NUM_VOICES];
    unsigned char mod_wheel;
    unsigned short pitch_bend;
    unsigned short controls[128];
} MidiState;

void midi_callback(double timeStamp, const unsigned char* message, size_t messageSize, void *userData);

#endif