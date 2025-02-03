#ifndef WAVEGRAPH_H
#define WAVEGRAPH_H
#include<oscillators.h>
#include<rng.h>
#include<midi.h>
#include<envelope.h>
enum NodeType{
    ADSR, MIDI, MODWHEEL, PITCHBEND, VELOCITY, WAVETABLE, TRIANGLE, SAW, SQUARE, SIN, POLYGON, OUTPUT, ADD, SUBTRACT, MULTIPLY, NUMBER, FILTER_LOWPASS, FILTER_HIGHPASS,FILTER_BANDPASS, FILTER_NOTCH, FILTER_APF, FILTER_PEAKEQ , COMB_FILTER, ALLPASS_FILTER, DIVIDE, WHITE_NOISE, PINK_NOISE, BROWN_NOISE, DELAY
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

WaveNode copyNode(WaveNode node);

void getNodeOutput(int node_idx, WaveNode* nodes, int num_nodes, int n, float* buffer, float dt);

WaveNode nodeNumber(float number);

WaveNode nodeWavetable(int frequency, Wavetable* table);
WaveNode nodeSin(int frequency);
WaveNode nodeTri(int frequency);
WaveNode nodeSqr(int frequency);
WaveNode nodeSaw(int frequency);
WaveNode nodePolygon(int frequency, int n);

WaveNode nodeAdd(int a, int b);
WaveNode nodeSub(int a, int b);
WaveNode nodeMul(int a, int b);
WaveNode nodeDiv(int a, int b);

WaveNode nodeFilterLowpass(int a, int fc, int resonance);

WaveNode nodeAllpass(int samples, int feedback, int delay);
WaveNode nodeAllpass(int samples, int alpha, int delay);

WaveNode nodeWhiteNoise();
WaveNode nodePinkNoise();
WaveNode nodeBrownNoise();

WaveNode nodeDelay(int samples, int delay_size, float decay);

WaveNode nodeMidi(int voice_idx, MidiState* state);
WaveNode nodeVelocity(int voice_idx, MidiState* state);
WaveNode nodeModWheel(MidiState* state);
WaveNode nodePitchBend(MidiState* MidiState);


WaveNode nodeAdsr(int trigger, ADSREnvelope* state);
#endif