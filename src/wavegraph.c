#include<wavegraph.h>
#include<filter.h>
#include<stdlib.h>
#include<rng.h>
#include<delay.h>
#include<string.h>

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
WaveNode copyNode(WaveNode* node){
    WaveNode res = *node;
    res.value = malloc(node->value_len);
    memcpy(res.value, node->value, node->value_len);
    return res;
}

// Currently won't work correctly if muliple outputs depend on one input. Do a toposort and a list of computed nodes
// See if node has multiple inputs cache that node.
void getNodeOutput(int node_idx, WaveNode* nodes, int num_nodes, int n, float* buffer, float dt){
    WaveNode node;
    if(node_idx < 0 || node_idx >= num_nodes){
        node = (WaveNode){.type=NUMBER, .value=&ZERO, .id=-1};
    }
    else{
        node = nodes[node_idx];
    }
    switch (node.type){
    case MIDI:
    {
        MidiState state = *(*(MidiState**)node.value);
        int voice_idx = *(int*)(((char*)node.value)+sizeof(MidiState*));
        float frequency = 261.625565*powf(powf(2.0f, 1.0f/12.0f), state.notes[0]-60.0f+12.0f*((float)((int)state.pitch_bend-16384))/16384.0f);
        for(int i = 0; i < n; i++){
            buffer[i] = frequency;
        }
        return;
    }
    case DELAY:
        //Incorrect
        {
            float samples[n];
            delay(samples, buffer, (Delay*)node.value, n);
            node.computed = 1;
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
        for(int i = 0; i < n; i++){
            float a = (rand_float((pcg32_random_t*)node.value)*2.0f)-1.0f;
            float b = (rand_float((pcg32_random_t*)node.value)*2.0f)-1.0f;
            buffer[i] = (a+b)*0.5;
            node.computed = 1;
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
                buffer[i] = osc_sin(frequency_buffer[i], nodes[node.inputs[1]].value, dt);
            }
            node.computed = 1;
            return;
            break;
        }
    case TRIANGLE:
        {
            // float frequency_buffer[n];
            // getNodeOutput(*(node.inputs+0), n, frequency_buffer, dt);
            // for(int i = 0; i < n; i++){
            //     buffer[i] = osc_tri(frequency_buffer[i], (float*)((node.inputs+1)->value), dt);
            // }
            return;
            break;
        }
    case SQUARE:
        {
            float frequency_buffer[n];
            getNodeOutput(node.inputs[0], nodes, num_nodes, n, frequency_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i] = osc_sqr(frequency_buffer[i], nodes[node.inputs[1]].value, dt);
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
                buffer[i] = osc_saw(frequency_buffer[i], nodes[node.inputs[1]].value, dt);
            }
            node.computed = 1;
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

WaveNode nodeComb(int samples, int alpha, int dampening, int delay){
    WaveNode node;
    node.type = COMB_FILTER;
    node.inputs = malloc(3*sizeof(int));
    node.inputs[0] = samples;
    node.inputs[1] = alpha;
    node.inputs[2] = dampening;
    node.num_inputs = 3;
    node.value = malloc(sizeof(CombFilter));
    node.computed = 0;
    node.cache = NULL;
    CombFilter f = comb(delay, 1.0f, 1.0f);
    node.id = COUNTER++;
    *((CombFilter*)node.value) = f;
    return node;
}

WaveNode nodeAllpass(int samples, int feedback, int delay){
    WaveNode node;
    node.type = ALLPASS_FILTER;
    node.inputs = malloc(2*sizeof(int));
    node.inputs[0] = samples;
    node.inputs[1] = feedback;
    node.value = malloc(sizeof(AllpassFilter));
    node.num_inputs = 2;
    node.computed = 0;
    node.cache = NULL;
    AllpassFilter f = init_all_pass(delay, 1.0f);
    *((AllpassFilter*)node.value) = f;
    node.id = COUNTER++;
    return node;
}

//Nodes don't copy values just the pointers
//Find a good way to seed random value
WaveNode nodeWhiteNoise(){
    WaveNode node;
    node.type = WHITE_NOISE;
    node.inputs = NULL;
    node.value = malloc(1*sizeof(pcg32_random_t));
    *((pcg32_random_t*)node.value) = make_rng(42, 63);
    node.num_inputs = 0;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodePinkNoise(){
    WaveNode node;
    node.type = PINK_NOISE;
    node.inputs = NULL;
    node.value = malloc(1*sizeof(pcg32_random_t));
    *((pcg32_random_t*)node.value) = make_rng(42, 63);
    node.num_inputs = 0;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

// Brown noise improper filter
WaveNode nodeBrownNoise(){
    WaveNode node;
    node.type = BROWN_NOISE;
    node.inputs = NULL;
    node.value = malloc(1*sizeof(pcg32_random_t));
    *((pcg32_random_t*)node.value) = make_rng(42, 63);
    node.num_inputs = 0;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeNumber(float number){
    WaveNode node;
    node.type = NUMBER;
    node.inputs = NULL;
    node.value = malloc(sizeof(float));
    float* ptr = (float*)node.value;
    *ptr = number;
    node.num_inputs = 0;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeWavetable(int frequency, Wavetable* table){
    WaveNode node;
    node.type = WAVETABLE;
    node.inputs = (int*)malloc(1*sizeof(int));
    node.inputs[0] = frequency;
    node.value = malloc(sizeof(Wavetable)+sizeof(float));
    ((Wavetable*)node.value)[0] = *table;
    ((float*)(((char*)node.value)+sizeof(Wavetable)))[0] = 0;
    node.num_inputs = 1;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeSin(int frequency, int phase){
    WaveNode node;
    node.type = SIN;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = frequency;
    node.inputs[1] = phase;
    node.value = NULL;
    node.num_inputs = 2;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeTri(int frequency, int phase){
    WaveNode node;
    node.type = TRIANGLE;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = frequency;
    node.inputs[1] = phase;
    node.value = NULL;
    node.num_inputs = 2;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeSqr(int frequency, int phase){
    WaveNode node;
    node.type = SQUARE;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = frequency;
    node.inputs[1] = phase;
    node.value = NULL;
    node.num_inputs = 2;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeSaw(int frequency, int phase){
    WaveNode node;
    node.type = SAW;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = frequency;
    node.inputs[1] = phase;
    node.value = NULL;
    node.num_inputs = 2;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeAdd(int a, int b){
    WaveNode node;
    node.type = ADD;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.value = NULL;
    node.num_inputs = 2;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeSub(int a, int b){
    WaveNode node;
    node.type = SUBTRACT;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.value = NULL;
    node.num_inputs = 2;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeMul(int a, int b){
    WaveNode node;
    node.type = MULTIPLY;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.value = NULL;
    node.num_inputs = 2;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeDiv(int a, int b){
    WaveNode node;
    node.type = DIVIDE;
    node.inputs = (int*)malloc(2*sizeof(int));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.value = NULL;
    node.num_inputs = 2;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}

WaveNode nodeDelay(int samples, int delay_size, float decay){
    WaveNode node;
    node.type = DELAY;
    node.inputs = malloc(1*sizeof(int));
    node.inputs[0] = samples;
    Delay d = init_delay(delay_size, decay);
    node.value = malloc(sizeof(Delay));
    *(Delay*)node.value = d;
    node.num_inputs = 1;
    node.computed = 0;
    node.cache = NULL;
    node.id = COUNTER++;
    return node;
}


WaveNode nodeMidi(int voice_idx, MidiState* state){
    WaveNode node;
    node.type = MIDI;
    node.inputs = NULL;
    node.value = malloc(sizeof(MidiState*)+sizeof(int));
    *(MidiState**)node.value = state;
    *(int*)(((char*)node.value)+sizeof(MidiState*)) = voice_idx;
    node.id = COUNTER++;
    node.num_inputs = 0;
    node.value_len = sizeof(MidiState*)+sizeof(int);
    return node;
}