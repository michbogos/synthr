#ifndef WAVEGRAPH_H
#define WAVEGRAPH_H
#include<oscillators.h>
enum NodeType{
    WAVETABLE, TRIANGLE, SAW, SQUARE, SIN, OUTPUT, ADD, SUBTRACT, MULTIPLY, NUMBER, FILTER, DIVIDE
};


struct node{
    enum NodeType type;
    struct node* inputs;
    //WaveNode* outputs;
    void* value;
};

typedef struct node WaveNode;

void getNodeOutput(WaveNode node, int n, float* buffer, float dt);

WaveNode nodeNumber(float number);
WaveNode nodeWavetable(WaveNode frequency, WaveNode phase, Wavetable* table);
WaveNode nodeSin(WaveNode frequency, WaveNode phase);
WaveNode nodeTri(WaveNode frequency, WaveNode phase);
WaveNode nodeSqr(WaveNode frequency, WaveNode phase);
WaveNode nodeSaw(WaveNode frequency, WaveNode phase);
WaveNode nodeAdd(WaveNode a, WaveNode b);
WaveNode nodeSub(WaveNode a, WaveNode b);
WaveNode nodeMul(WaveNode a, WaveNode b);
WaveNode nodeDiv(WaveNode a, WaveNode b);

#endif