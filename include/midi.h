#ifndef MIDI_H
#define MIDI_H

#define NUM_VOICES 16
#define NUM_CONTROLS 8

#include <stddef.h>


typedef struct {
    //Keep MIDI value as well as wether turned on or not
    unsigned char notes[NUM_VOICES];
    unsigned char velocities[NUM_VOICES];
    unsigned char is_on[NUM_VOICES];
    unsigned char mod_wheel;
    unsigned short pitch_bend;
    unsigned short controls[NUM_CONTROLS];
} MidiState;

void midi_callback(double timeStamp, const unsigned char* message, size_t messageSize, void *userData);

void add_control();

#endif