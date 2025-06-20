#include<wavegraph.h>
#include<filter.h>
#include<stdlib.h>
#include<rng.h>
#include<delay.h>
#include<string.h>
#include<effects.h>
#include<reverb.h>

#define NULL ((void*)0)
int COUNTER = 0;
float ZERO = 0.0f;

// unsigned int countNodes(WaveNode node, ){
//     int count = 1;
//     for(int i = 0; i < node.num_inputs; i++){
//         count += countNodes(node.inputs[i]);
//     }
//     return count;
// }

// void toposort(WaveNode node, WaveNode* res){
//     WaveNode* res
// }

//Copies just one layer of pointer indirection
WaveNode copyNode(WaveNode node){
    WaveNode res = node;
    if(node.value != NULL && node.value_len > 0){
        res.value = malloc(node.value_len);
        memcpy(res.value, node.value, node.value_len);
    }
    return res;
}

// Currently won't work correctly if muliple outputs depend on one input. Do a toposort and a list of computed nodes
// See if node has multiple inputs cache that node.
void getNodeOutput(int node_idx, WaveNode* nodes, int num_nodes, int n, float* buffer, float dt){
    WaveNode node = nodeDefault();
    if(node_idx < 0 || node_idx >= num_nodes){
        node = (WaveNode){.type=NUMBER, .value=&ZERO, .id=-1, .computed=-1};
    }
    else{
        node = nodes[node_idx];
    }
    switch (node.type){
    case FILTER_LOWPASS:
    case FILTER_HIGHPASS:
    case FILTER_BANDPASS:
    case FILTER_NOTCH:
    case FILTER_APF:
    //case FILTER_PEAKEQ:
    {
        if(node.inputs[0] < 0 || node.inputs[1] < 0 || node.inputs[2] < 0){
            return;
        }
        float a[n];
        float fc[n];
        float resonance[n];
        getNodeOutput(node.inputs[0], nodes, num_nodes, n, a, dt);
        getNodeOutput(node.inputs[1], nodes, num_nodes, n, fc, dt);
        getNodeOutput(node.inputs[2], nodes, num_nodes, n, resonance, dt);

        for(int i = 0; i < n; i++){
            buffer[i] = filter(a[i], 1.0f/dt, node.value, fc[i], resonance[i], 1.0);
        }
        return;
        break;
    }
    case ADSR:
    {
        float trigger[n];
        getNodeOutput(node.inputs[0], nodes, num_nodes, n, trigger, dt);
        if(trigger[0] > 0 && !(((ADSREnvelope*)node.value)->gate > 0)){
            ((ADSREnvelope*)node.value)->state = 1;
        }
        if(!(trigger[0] > 0) && ((ADSREnvelope*)node.value)->gate > 0){
            ((ADSREnvelope*)node.value)->state = 4;
        }
        for(int i = 0; i < n; i++){
            buffer[i] = gen_adsr_envelope(node.value);
        }
        ((ADSREnvelope*)node.value)->gate = trigger[n-1];
        return;
        break;
    }
    case MIDI_GATE:
    {
        MidiState state = *(*(MidiState**)node.value);
        int voice_idx = *(int*)(((char*)node.value)+sizeof(MidiState*));
        for(int i = 0; i < n; i++){
            buffer[i] = state.is_on[voice_idx];
        }
        return;
        break;
    }
    case MIDI_PITCH:
    {
        MidiState state = *(*(MidiState**)node.value);
        int voice_idx = *(int*)(((char*)node.value)+sizeof(MidiState*));
        float frequency = (261.625565*powf(powf(2.0f, 1.0f/12.0f), state.notes[voice_idx]-60.0f+12.0f*((float)((int)state.pitch_bend-16384))/16384.0f));
        for(int i = 0; i < n; i++){
            buffer[i] = frequency;
        }
        return;
        break;
    }
    case MIDI_CONTROL:
    {
        MidiState state = *(*(MidiState**)node.value);
        int control_idx = *(int*)(((char*)node.value)+sizeof(MidiState*)) > 127 ? 127 : *(int*)(((char*)node.value)+sizeof(MidiState*));
        control_idx = control_idx < -1 ? 0 : control_idx;
        for(int i = 0; i < n; i++){
            buffer[i] = state.controls[control_idx]/127.0f;
        }
        return;
        break;
    }
    case VELOCITY:
    {
        MidiState state = *(*(MidiState**)node.value);
        int voice_idx = *(int*)(((char*)node.value)+sizeof(MidiState*));
        for(int i = 0; i < n; i++){
            buffer[i] = (float)state.velocities[voice_idx]/127.0f;
        }
        return;
        break;
    }

    case MODWHEEL:
    {
        MidiState state = *(*(MidiState**)node.value);
        for(int i = 0; i < n; i++){
            buffer[i] = (float)state.mod_wheel/127.0f;
        }
        return;
        break;
    }

    case PITCHBEND:
    {
        MidiState state = *(*(MidiState**)node.value);
        for(int i = 0; i < n; i++){
            buffer[i] = (float)state.pitch_bend/32767.0f;
        }
        return;
        break;
    }

    case DISTORTION:
    {
        float input[n];
        getNodeOutput(node.inputs[0], nodes, num_nodes, n, input, dt);
        for(int i = 0; i < n; i++){
            buffer[i] = distortion_foldback(input[i], 0.5);
        }
        return;
        break;
    }

    case BITCRUSHER:
    {
        float input[n];
        float bits[n];
        getNodeOutput(node.inputs[0], nodes, num_nodes, n, input, dt);
        getNodeOutput(node.inputs[1], nodes, num_nodes, n, bits, dt);
        for(int i = 0; i < n; i++){
            buffer[i] = bit_crusher(input[i], abs(bits[i]));
        }
        return;
        break;
    }

    case DELAY:
        //Incorrect
        {
            float samples[n];
            float delay_size[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, samples, dt);
            getNodeOutput(node.inputs[1], nodes, num_nodes, n, delay_size, dt);
            if(delay_size[0] != 0){
                set_delay((Delay*)node.value, abs(delay_size[0]));
            }
            delay(samples, buffer, (Delay*)node.value, n);
        }
        return;
        break;
    case ALLPASS_FILTER:
        {
            float alpha[n];
            float samples[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, alpha, dt);
            getNodeOutput(node.inputs[1], nodes, num_nodes, n, samples, dt);
            for(int i = 0; i < n; i++){
                ((AllpassFilter*)node.value)->feedback = alpha[i];
                buffer[i] = filter_allpass((AllpassFilter*)node.value, samples[i]);
            }
            node.computed = 1;
            return;
            break;
        }
    case COMB_FILTER:
        {
            float alpha[n];
            float samples[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, alpha, dt);
            getNodeOutput(node.inputs[1], nodes, num_nodes, n, samples, dt);
            for(int i = 0; i < n; i++){
                ((CombFilter*)node.value)->alpha = alpha[i];
                buffer[i] = filter_comb((CombFilter*)node.value, samples[i]);
            }
            node.computed = 1;
            return;
            break;
        }
    case REVERB:
        {
            float samples[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, samples, dt);
            fv_process(node.value, samples, n);
            for(int i = 0; i < n; i++){
                buffer[i] = samples[i];
            }
            node.computed = 1;
            return;
            break;
        }
    case WHITE_NOISE:
        for(int i = 0; i < n; i++){
            buffer[i] = (rand_float((pcg32_random_t*)node.value)*2.0f)-1.0f;
        }
        node.computed = 1;
        return;
        break;
    case PINK_NOISE:
        for(int i = 0; i < n; i++){
            buffer[i] = (rand_float((pcg32_random_t*)node.value)*2.0f)-1.0f;
            float b0 = 0.99765 * b0 + buffer[i] * 0.0990460;
            float b1 = 0.96300 * b1 + buffer[i] * 0.2965164;
            float b2 = 0.57000 * b2 + buffer[i] * 1.0526913;
            buffer[i] = (b0 + b1 + b2 + buffer[i] * 0.1848)*0.2;
            node.computed = 1;
        }
        return;
        break;
    case BROWN_NOISE:
    {
        float* c = (float*)((char*)node.value+sizeof(pcg32_random_t));
        float* last = (float*)((char*)node.value+sizeof(pcg32_random_t)+sizeof(float));
        for(int i = 0; i < n; i++){
            float val = (2*rand_float(node.value)-1);
            buffer[i] = *last+0.5*val;
            *last = buffer[i];
        }
    }
        return;
        break;
    case NUMBER:
        for(int i = 0; i < n; i++){
            buffer[i] = ((float*)(node.value))[0];
        }
        break;
        return;
    
    case WAVETABLE:
        {
            float frequency_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_tbl(frequency_buffer[i], (float*)((char*)node.value+sizeof(Wavetable)), dt, (Wavetable*)node.value);
            }
            node.computed = 1;
            return;
            break;
        }
    case SIN:
        {
            float frequency_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_sin(frequency_buffer[i], node.value, dt);
            }
            node.computed = 1;
            return;
            break;
        }
    case TRIANGLE:
        {
            float frequency_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_tri(frequency_buffer[i], node.value, dt);
            }
            return;
            break;
        }
    case SQUARE:
        {
            float frequency_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_sqr(frequency_buffer[i], node.value, dt);
            }
            node.computed = 1;
            return;
            break;
        }
    case SAW:
        {
            float frequency_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_saw(frequency_buffer[i], node.value, dt);
            }
            node.computed = 1;
            return;
            break;
        }
    case POLYGON:
        {
            float frequency_buffer[n];
            float n_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, frequency_buffer, dt);
            getNodeOutput(node.inputs[1], nodes, num_nodes, n, n_buffer, dt);
            for(int i = 0; i < n; i ++){
                buffer[i] = osc_ply(n_buffer[i], frequency_buffer[i], node.value, dt);
            }
            return;
            break;
        }
    case ADD:
        {
            float a_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, a_buffer, dt);
            float b_buffer[n];
            getNodeOutput(node.inputs[1], nodes, num_nodes, n, b_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i]=a_buffer[i]+b_buffer[i];
            }
            node.computed = 1;
            return;
            break;
        }
    case SUBTRACT:
        {
            float a_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, a_buffer, dt);
            float b_buffer[n];
            getNodeOutput(node.inputs[1], nodes, num_nodes, n, b_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i]=a_buffer[i]-b_buffer[i];
            }
            node.computed = 1;
            return;
            break;
        }
    case MULTIPLY:
        {
            float a_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, a_buffer, dt);
            float b_buffer[n];
            getNodeOutput(node.inputs[1], nodes, num_nodes, n, b_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i]=a_buffer[i]*b_buffer[i];
            }
            node.computed = 1;
            return;
            break;
        }
    case DIVIDE:
        {
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, buffer, dt);
            float b_buffer[n];
            getNodeOutput(node.inputs[1], nodes, num_nodes, n, b_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i]=buffer[i]/b_buffer[i];
            }
            node.computed = 1;
            return;
            break;
        }
    case DIFFERENTIATE:
        {
            float input[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, input, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = (input[i]-*(float*)node.value)/dt;
                *(float*)node.value = input[i];
            }
            node.computed = 1;
            return;
            break;
        }
    default:
        return;
    }
}

// void clearNodeCache(WaveNode* node){
//     node->computed = 0;
//     for(int i = 0; i < node->num_inputs; i++){
//         clearNodeCache(node->inputs+i);
//     }
//     return;
// }

WaveNode nodeDefault(){
    WaveNode node;
    node.value = NULL;
    node.cache = NULL;
    node.value_len = 0;
    node.id = COUNTER++;
    node.num_inputs = 0;
    node.computed = 0;
    node.inputs = NULL;
    return node;
}

WaveNode nodeComb(int samples, int alpha, int dampening, int delay){
    WaveNode node = nodeDefault();
    node.type = COMB_FILTER;
    node.inputs = malloc(3*sizeof(int));
    node.inputs[0] = samples;
    node.inputs[1] = alpha;
    node.inputs[2] = dampening;
    node.num_inputs = 3;
    node.value = malloc(sizeof(CombFilter));
    CombFilter f = comb(delay, 1.0f, 1.0f);
    *((CombFilter*)node.value) = f;
    return node;
}

WaveNode nodeAllpass(int samples, int feedback, int delay){
    WaveNode node = nodeDefault();
    node.type = ALLPASS_FILTER;
    node.inputs = malloc(2*sizeof(int));
    node.inputs[0] = samples;
    node.inputs[1] = feedback;
    node.value = malloc(sizeof(AllpassFilter));
    node.num_inputs = 2;
    AllpassFilter f = init_all_pass(delay, 1.0f);
    *((AllpassFilter*)node.value) = f;
    return node;
}

//Nodes don't copy values just the pointers
//Find a good way to seed random value
WaveNode nodeWhiteNoise(){
    WaveNode node = nodeDefault();
    node.type = WHITE_NOISE;
    node.value = malloc(16);
    node.value_len = 16;
    *((pcg32_random_t*)node.value) = make_rng(42, 63);
    return node;
}

WaveNode nodePinkNoise(){
    WaveNode node = nodeDefault();
    node.type = PINK_NOISE;
    node.value = malloc(1*sizeof(pcg32_random_t));
    *((pcg32_random_t*)node.value) = make_rng(42, 63);
    return node;
}

// Brown noise improper filter
WaveNode nodeBrownNoise(){
    WaveNode node = nodeDefault();
    node.type = BROWN_NOISE;
    node.value = malloc(1*sizeof(pcg32_random_t)+1*sizeof(float)+sizeof(Biquad));
    node.value_len = 1*sizeof(pcg32_random_t)+1*sizeof(float)+sizeof(Biquad);
    *((pcg32_random_t*)node.value) = make_rng(42, 63);
    *(float*)((char*)node.value+sizeof(pcg32_random_t)) = 0.0;
    *(Biquad*)((char*)node.value+sizeof(pcg32_random_t)+sizeof(float)) = biquad(HIGHPASS);
    return node;
}

WaveNode nodeNumber(float number){
    WaveNode node = nodeDefault();
    node.type = NUMBER;
    node.value = malloc(sizeof(float));
    float* ptr = (float*)node.value;
    *ptr = number;
    node.value_len = sizeof(float);
    return node;
}

WaveNode nodeWavetable(int frequency, Wavetable* table){
    WaveNode node = nodeDefault();
    node.type = WAVETABLE;
    node.inputs = (int*)malloc(1*sizeof(int));
    node.inputs[0] = frequency;
    node.value = malloc(sizeof(Wavetable)+sizeof(float));
    node.value_len = sizeof(Wavetable)+sizeof(float);
    ((Wavetable*)node.value)[0] = *table;
    ((float*)(((char*)node.value)+sizeof(Wavetable)))[0] = 0;
    node.num_inputs = 1;
    return node;
}

WaveNode nodeSin(int frequency){
    WaveNode node = nodeDefault();
    node.type = SIN;
    node.inputs = (int*)malloc(1*sizeof(int));
    node.inputs[0] = frequency;
    node.value = malloc(sizeof(float));
    *(float*)(node.value) = 0.0f;
    node.value_len = sizeof(float);
    node.num_inputs = 1;
    return node;
}

WaveNode nodeTri(int frequency){
    WaveNode node = nodeDefault();
    node.type = TRIANGLE;
    node.inputs = (int*)malloc(1*sizeof(int));
    node.inputs[0] = frequency;
    node.value = malloc(sizeof(float));
    *(float*)(node.value) = 0.0f;
    node.value_len = sizeof(float);
    node.num_inputs = 1;
    return node;
}

WaveNode nodeSqr(int frequency){
    WaveNode node = nodeDefault();
    node.type = SQUARE;
    node.inputs = (int*)malloc(1*sizeof(int));
    node.inputs[0] = frequency;
    node.value = malloc(sizeof(float));
    *(float*)(node.value) = 0.0f;
    node.value_len = sizeof(float);
    node.num_inputs = 1;
    return node;
}

WaveNode nodeSaw(int frequency){
    WaveNode node = nodeDefault();
    node.type = SAW;
    node.inputs = (int*)malloc(1*sizeof(int));
    node.inputs[0] = frequency;
    node.value = malloc(sizeof(float));
    *(float*)(node.value) = 0.0f;
    node.value_len = sizeof(float);
    node.num_inputs = 1;
    return node;
}

WaveNode nodePolygon(int frequency, int n){
    WaveNode node = nodeDefault();
    node.type = POLYGON;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = frequency;
    node.inputs[1] = n;
    node.value = malloc(sizeof(float));
    *(float*)(node.value) = 0.0f;
    node.value_len = sizeof(float);
    node.num_inputs = 2;
    return node;
}

WaveNode nodeAdd(int a, int b){
    WaveNode node = nodeDefault();
    node.type = ADD;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.num_inputs = 2;
    return node;
}

WaveNode nodeSub(int a, int b){
    WaveNode node = nodeDefault();
    node.type = SUBTRACT;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.num_inputs = 2;
    return node;
}

WaveNode nodeMul(int a, int b){
    WaveNode node = nodeDefault();
    node.type = MULTIPLY;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.num_inputs = 2;
    return node;
}

WaveNode nodeDiv(int a, int b){
    WaveNode node = nodeDefault();
    node.type = DIVIDE;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.num_inputs = 2;
    return node;
}

WaveNode nodeDifferentiate(int a){
    WaveNode node = nodeDefault();
    node.type = DIFFERENTIATE;
    node.inputs = (int*) malloc(1*sizeof(int));
    node.inputs[0] = a;
    node.value = malloc(1*sizeof(float));
    node.value_len = sizeof(float);
    return node;
}

WaveNode nodeDelay(int input, int delay_size, float decay){
    WaveNode node = nodeDefault();
    node.type = DELAY;
    node.inputs = malloc(2*sizeof(int));
    node.inputs[0] = input;
    node.inputs[1] = delay_size;
    Delay d = init_delay(48000, decay);
    node.value = malloc(sizeof(Delay));
    node.value_len = sizeof(Delay);
    *(Delay*)node.value = d;
    node.num_inputs = 2;
    return node;
}

WaveNode nodeDistortion(int input){
    WaveNode node = nodeDefault();
    node.type = DISTORTION;
    node.inputs = malloc(1*sizeof(int));
    node.inputs[0] = input;
    node.num_inputs = 1;
    return node;
}

WaveNode nodeBitcrusher(int input, int bits){
    WaveNode node = nodeDefault();
    node.type = BITCRUSHER;
    node.inputs = malloc(2*sizeof(int));
    node.inputs[0] = input;
    node.inputs[1] = bits;
    node.num_inputs = 2;
    return node;
}

WaveNode nodeMidiGate(int voice_idx, MidiState* state){
    WaveNode node = nodeDefault();
    node.type = MIDI_GATE;
    node.value = malloc(sizeof(MidiState*)+sizeof(int));
    *(MidiState**)node.value = state;
    *(int*)(((char*)node.value)+sizeof(MidiState*)) = voice_idx;
    node.value_len = sizeof(MidiState*)+sizeof(int);
    return node;
}

WaveNode nodeMidiPitch(int voice_idx, MidiState* state){
    WaveNode node = nodeDefault();
    node.type = MIDI_PITCH;
    node.value = malloc(sizeof(MidiState*)+sizeof(int));
    *(MidiState**)node.value = state;
    *(int*)(((char*)node.value)+sizeof(MidiState*)) = voice_idx;
    node.value_len = sizeof(MidiState*)+sizeof(int);
    return node;
}

WaveNode nodeMidiControl(int control_idx, MidiState* state){
    WaveNode node = nodeDefault();
    node.type = MIDI_CONTROL;
    node.value = malloc(sizeof(MidiState*)+sizeof(int));
    *(MidiState**)node.value = state;
    *(int*)(((char*)node.value)+sizeof(MidiState*)) = control_idx;
    node.value_len = sizeof(MidiState*)+sizeof(int);
    return node;
}

WaveNode nodeVelocity(int voice_idx, MidiState* state){
    WaveNode node = nodeDefault();
    node.type = VELOCITY;
    node.value = malloc(sizeof(MidiState*)+sizeof(int));
    *(MidiState**)node.value = state;
    *(int*)(((char*)node.value)+sizeof(MidiState*)) = voice_idx;
    node.value_len = sizeof(MidiState*)+sizeof(int);
    return node;
}

WaveNode nodeModWheel(MidiState* state){
    WaveNode node = nodeDefault();
    node.type = MODWHEEL;
    node.value = malloc(sizeof(MidiState*));
    *(MidiState**)node.value = state;
    node.value_len = sizeof(MidiState*);
    return node;
}

WaveNode nodePitchBend(MidiState* state){
    WaveNode node = nodeDefault();
    node.type = PITCHBEND;
    node.value = malloc(sizeof(MidiState*));
    *(MidiState**)node.value = state;
    node.value_len = sizeof(MidiState*);
    return node;
}

WaveNode nodeAdsr(int trigger, ADSREnvelope* state){
    WaveNode node = nodeDefault();
    node.type = ADSR;
    node.inputs = malloc(1*sizeof(int));
    node.num_inputs = 1;
    node.value = malloc(sizeof(ADSREnvelope));
    memcpy(node.value, state, sizeof(ADSREnvelope));
    node.value_len = sizeof(ADSREnvelope);
    node.inputs[0] = trigger;
    return node;
 }

WaveNode nodeFilterLowpass(int a, int fc, int resonance){
    Biquad bq = biquad(LOWPASS);
    WaveNode node = nodeDefault();
    node.type = FILTER_LOWPASS;
    node.value = malloc(sizeof(Biquad));
    node.value_len = sizeof(Biquad);
    *(Biquad*)node.value = bq;
    node.num_inputs = 3;
    node.inputs = malloc(sizeof(int)*3);
    node.inputs[0] = a;
    node.inputs[1] = fc;
    node.inputs[2] = resonance;
    return node;
 }

WaveNode nodeFilterHighpass(int a, int fc, int resonance){
    Biquad bq = biquad(HIGHPASS);
    WaveNode node = nodeDefault();
    node.type = FILTER_HIGHPASS;
    node.value = malloc(sizeof(Biquad));
    node.value_len = sizeof(Biquad);
    *(Biquad*)node.value = bq;
    node.num_inputs = 3;
    node.inputs = malloc(sizeof(int)*3);
    node.inputs[0] = a;
    node.inputs[1] = fc;
    node.inputs[2] = resonance;
    return node;
}

WaveNode nodeFilterBandpass(int a, int fc, int resonance){
    Biquad bq = biquad(BANDPASSQ);
    WaveNode node = nodeDefault();
    node.type = FILTER_BANDPASS;
    node.value = malloc(sizeof(Biquad));
    node.value_len = sizeof(Biquad);
    *(Biquad*)node.value = bq;
    node.num_inputs = 3;
    node.inputs = malloc(sizeof(int)*3);
    node.inputs[0] = a;
    node.inputs[1] = fc;
    node.inputs[2] = resonance;
    return node;
 }

WaveNode nodeFilterAPF(int a, int fc, int resonance){
    Biquad bq = biquad(APF);
    WaveNode node = nodeDefault();
    node.type = FILTER_APF;
    node.value = malloc(sizeof(Biquad));
    node.value_len = sizeof(Biquad);
    *(Biquad*)node.value = bq;
    node.num_inputs = 3;
    node.inputs = malloc(sizeof(int)*3);
    node.inputs[0] = a;
    node.inputs[1] = fc;
    node.inputs[2] = resonance;
    return node;
 }

 WaveNode nodeReverb(int input){
    WaveNode node = nodeDefault();
    node.num_inputs = 1;
    node.inputs = malloc(1*sizeof(int));
    node.inputs[0] = input;
    node.value = malloc(1*sizeof(fv_Context));
    fv_init(node.value);
    fv_set_samplerate(node.value, 48000);
    node.type = REVERB;
    node.value_len = sizeof(fv_Context);
    return node;
 }