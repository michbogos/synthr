#include<midi.h>
#include<stdio.h>

void midi_callback(double timeStamp, const unsigned char* message, size_t messageSize, void *userData){
    MidiState* state = (MidiState*)userData;
    switch ((message[0]&0b11110000)>>4){
        //Note On
        case 9:
            {
                for(int i = 0; i < NUM_VOICES; i++){
                    if(state->notes[i]==0){
                        state->notes[i] = message[1];
                        state->velocities[i] = message[2];
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
                    state->velocities[i] = 0;
                    printf("Turning off");
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
        // Other midi controller controls
        case 11:
            switch (message[1])
            {
                // Modwheel
                case 1:
                    state->mod_wheel = message[2];
                    return;
                    break;
                
                default:
                    break;
            }
            break;
        

        default:
            printf("Unknown MIDI event. First byte: %d, Second byte: %d, Third byte: %d\n", message[0], message[1], message[2]);
            return;
            break;
    }
    return;
}