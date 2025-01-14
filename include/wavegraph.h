#ifndef WAVEGRAPH_H
#define WAVEGRAPH_H
#include<oscillators.h>
#include<rng.h>
enum NodeType{
    WAVETABLE, TRIANGLE, SAW, SQUARE, SIN, OUTPUT, ADD, SUBTRACT, MULTIPLY, NUMBER, FILTER, COMB_FILTER, ALLPASS_FILTER, DIVIDE, WHITE_NOISE, PINK_NOISE, BROWN_NOISE, DELAY
};


struct node{
    enum NodeType type;
    int* inputs;
    unsigned int num_inputs;
    float* cache;
    char computed;
    int id;
    //WaveNode* outputs;
    void* value;
    unsigned int value_len; // Need to implement for all nodes
};

typedef struct node WaveNode;

void getNodeOutput(int node_idx, WaveNode* nodes, int num_nodes, int n, float* buffer, float dt);

WaveNode nodeNumber(float number);
WaveNode nodeWavetable(int frequency, Wavetable* table);
WaveNode nodeSin(int frequency, int phase);
WaveNode nodeTri(int frequency, int phase);
WaveNode nodeSqr(int frequency, int phase);
WaveNode nodeSaw(int frequency, int phase);
WaveNode nodeAdd(int a, int b);
WaveNode nodeSub(int a, int b);
WaveNode nodeMul(int a, int b);
WaveNode nodeDiv(int a, int b);

WaveNode nodeAllpass(int samples, int feedback, int delay);
WaveNode nodeAllpass(int samples, int alpha, int delay);

WaveNode nodeWhiteNoise();
WaveNode nodePinkNoise();
WaveNode nodeBrownNoise();

WaveNode nodeDelay(int samples, int delay_size, float decay);
#endif