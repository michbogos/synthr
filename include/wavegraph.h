#ifndef WAVEGRAPH_H
#define WAVEGRAPH_H
#include<oscillators.h>
enum NodeType{
    WAVETABLE, TRIANGLE, SAW, SQUARE, SIN, OUTPUT, ADD, SUBTRACT, MULTIPLY, NUMBER, FILTER
};


struct node{
    enum NodeType type;
    struct node* inputs;
    //WaveNode* outputs;
    void* value;
};

typedef struct node WaveNode;

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
                buffer[i] = osc_tbl(frequency_buffer[i], (float*)((node.inputs+1)->value), dt, (Wavetable*)node.value);
            }
            return;
            break;
        }
    case SIN:
        {
            float frequency_buffer[n];
            getNodeOutput(*(node.inputs+0), n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_sin(frequency_buffer[i], (float*)((node.inputs+1)->value), dt);
            }
            return;
            break;
        }
    case TRIANGLE:
        {
            float frequency_buffer[n];
            getNodeOutput(*(node.inputs+0), n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_tri(frequency_buffer[i], (float*)((node.inputs+1)->value), dt);
            }
            return;
            break;
        }
    case SQUARE:
        {
            float frequency_buffer[n];
            getNodeOutput(*(node.inputs+0), n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_sqr(frequency_buffer[i], (float*)((node.inputs+1)->value), dt);
            }
            return;
            break;
        }
    case SAW:
        {
            float frequency_buffer[n];
            getNodeOutput(*(node.inputs+0), n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_saw(frequency_buffer[i], (float*)((node.inputs+1)->value), dt);
            }
            return;
            break;
        }
    default:
        return;
    }
}

WaveNode nodeNumber(float number){
    WaveNode node;
    node.type = NUMBER;
    node.inputs = NULL;
    node.value = malloc(sizeof(float));
    float* ptr = (float*)node.value;
    ptr[0] = number;
    return node;
}

WaveNode nodeWavetable(WaveNode frequency, WaveNode phase, Wavetable* table){
    WaveNode node;
    node.type = WAVETABLE;
    node.inputs = (WaveNode*)malloc(2*sizeof(WaveNode));
    node.inputs[0] = frequency;
    node.inputs[1] = phase;
    node.value = table;
    return node;
}

WaveNode nodeSin(WaveNode frequency, WaveNode phase){
    WaveNode node;
    node.type = SIN;
    node.inputs = (WaveNode*)malloc(2*sizeof(WaveNode));
    node.inputs[0] = frequency;
    node.inputs[1] = phase;
    node.value = NULL;
    return node;
}

WaveNode nodeTri(WaveNode frequency, WaveNode phase){
    WaveNode node;
    node.type = TRIANGLE;
    node.inputs = (WaveNode*)malloc(2*sizeof(WaveNode));
    node.inputs[0] = frequency;
    node.inputs[1] = phase;
    node.value = NULL;
    return node;
}

WaveNode nodeSqr(WaveNode frequency, WaveNode phase){
    WaveNode node;
    node.type = SQUARE;
    node.inputs = (WaveNode*)malloc(2*sizeof(WaveNode));
    node.inputs[0] = frequency;
    node.inputs[1] = phase;
    node.value = NULL;
    return node;
}

WaveNode nodeSaw(WaveNode frequency, WaveNode phase){
    WaveNode node;
    node.type = SAW;
    node.inputs = (WaveNode*)malloc(2*sizeof(WaveNode));
    node.inputs[0] = frequency;
    node.inputs[1] = phase;
    node.value = NULL;
    return node;
}

#endif