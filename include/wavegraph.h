#ifndef WAVEGRAPH_H
#define WAVEGRAPH_H
#include<oscillators.h>
enum NodeType{
    WAVETABLE, TRIANGLE, SAW, SQUARE, SIN, OUTPUT, ADD, SUBTRACT, MULTIPLY, NUMBER, FILTER
};

typedef struct WaveNode{
    NodeType type;
    WaveNode* inputs;
    //WaveNode* outputs;
    void* value;
};

void getNodeOutput(WaveNode node, int n, float* buffer, float dt){
    switch (node.type){
    case NUMBER:
        for(int i = 0; i < n; i++){
            buffer[i] = *(float*)(node.value);
        }
        return;
        break;
    
    case WAVETABLE:
        {
            float frequency_buffer[n];
            getNodeOutput(*(node.inputs+0), n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_tbl(frequency_buffer[i], (float*)(node.inputs->value), dt, (Wavetable*)node.value);
            }
            return;
            break;
        }
    default:
        return;
    }
}

#endif