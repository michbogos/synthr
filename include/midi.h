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

void midi_callback(double timeStamp, const unsigned char* message, size_t messageSize, void *userData){
    MidiState* state = (MidiState*)userData;
    switch ((message[0]&0b11110000)>>4){
        //Note On
        case 9:
            {
                for(int i = 0; i < NUM_VOICES; i++){
                    if(state->notes[i]==0){
                        state->notes[i] = message[1];
                        //state->velocities[i] = message[2];
                        return;
                    }
                }
                break;
            }
        // Note Off
        case 8:
        {
            for(int i = 0; i < NUM_VOICES; i++){
                if(state->notes[i]==message[1]){
                    state->notes[i] = 0;
                    //state->velocities[i] = 0;
                    return;
                }
            }
            break;
        }
        // Pitch Bend
        case 14:
        {
            state->pitch_bend = message[1]+(message[2]<<8);
            return;
            break;
        }
        default:
            return;
    }
    return;
}

#endif