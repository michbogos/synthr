#ifndef MIDI_H
#define MIDI_H

#define NUM_VOICES 16

#include <stddef.h>


typedef struct {
    unsigned char notes[NUM_VOICES];
    unsigned char velocities[NUM_VOICES];
    unsigned char mod_wheel;
    unsigned short pitch_bend;
} MidiState;

void midi_callback(double timeStamp, const unsigned char* message, size_t messageSize, void *userData);

#endif