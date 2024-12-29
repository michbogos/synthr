#ifndef WAVEGRAPH_H
#define WAVEGRAPH_H
#include<oscillators.h>
#include<rng.h>
enum NodeType{
    WAVETABLE, TRIANGLE, SAW, SQUARE, SIN, OUTPUT, ADD, SUBTRACT, MULTIPLY, NUMBER, FILTER, COMB_FILTER, DIVIDE, WHITE_NOISE, PINK_NOISE, BROWN_NOISE, DELAY
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

WaveNode nodeComb(WaveNode samples, WaveNode alpha, int delay);

WaveNode nodeWhiteNoise();
WaveNode nodePinkNoise();
WaveNode nodeBrownNoise();

WaveNode nodeDelay(int delay_size, float decay);
#endif