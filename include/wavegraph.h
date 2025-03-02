#ifndef WAVEGRAPH_H
#define WAVEGRAPH_H
#include<oscillators.h>
#include<rng.h>
#include<midi.h>
#include<envelope.h>
enum NodeType{
    ADSR=0, 
    MIDI_GATE=1, 
    MIDI_PITCH=2,  
    MODWHEEL=3,
    PITCHBEND=4,
    VELOCITY=5,
    WAVETABLE=6,
    TRIANGLE=7,
    SAW=8,
    SQUARE=9,
    SIN=10,
    POLYGON=11,
    OUTPUT=12,
    ADD=13, SUBTRACT, MULTIPLY, NUMBER, FILTER_LOWPASS, FILTER_HIGHPASS,FILTER_BANDPASS, FILTER_NOTCH, FILTER_APF, FILTER_PEAKEQ , COMB_FILTER, ALLPASS_FILTER, DIVIDE, WHITE_NOISE, PINK_NOISE, BROWN_NOISE, DELAY, DISTORTION, BITCRUSHER, MIDI_CONTROL
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
WaveNode nodeDistortion(int input);
WaveNode nodeBitcrusher(int input, int bits);

WaveNode nodeMidiGate(int voice_idx, MidiState* state);
WaveNode nodeMidiPitch(int voice_idx, MidiState* state);
WaveNode nodeMidiControl(int control_idx, MidiState* state);
WaveNode nodeVelocity(int voice_idx, MidiState* state);
WaveNode nodeModWheel(MidiState* state);
WaveNode nodePitchBend(MidiState* MidiState);


WaveNode nodeAdsr(int trigger, ADSREnvelope* state);
#endif