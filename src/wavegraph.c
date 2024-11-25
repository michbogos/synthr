#include<wavegraph.h>
#include<stdlib.h>
#include<rng.h>

#define NULL ((void*)0)

void getNodeOutput(WaveNode node, int n, float* buffer, float dt){
    switch (node.type){
    case WHITE_NOISE:
        for(int i = 0; i < n; i++){
            buffer[i] = (rand_float((pcg32_random_t*)node.value)*2.0f)-1.0f;
        }
        return;
        break;
    case PINK_NOISE:
        for(int i = 0; i < n; i++){
            buffer[i] = (rand_float((pcg32_random_t*)node.value)*2.0f)-1.0f;
            float b0 = 0.99765 * b0 + buffer[i] * 0.0990460;
            float b1 = 0.96300 * b1 + buffer[i] * 0.2965164;
            float b2 = 0.57000 * b2 + buffer[i] * 1.0526913;
            buffer[i] = (b0 + b1 + b2 + buffer[i] * 0.1848)*0.2;
        }
        return;
        break;
    case BROWN_NOISE:
        for(int i = 0; i < n; i++){
            float a = (rand_float((pcg32_random_t*)node.value)*2.0f)-1.0f;
            float b = (rand_float((pcg32_random_t*)node.value)*2.0f)-1.0f;
            buffer[i] = (a+b)*0.5;
        }
        return;
        break;
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
    case ADD:
        {
            float a_buffer[n];
            getNodeOutput(*(node.inputs), n, a_buffer, dt);
            float b_buffer[n];
            getNodeOutput(*(node.inputs+1), n, b_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i]=a_buffer[i]+b_buffer[i];
            }
            return;
            break;
        }
    case SUBTRACT:
        {
            float a_buffer[n];
            getNodeOutput(*(node.inputs), n, a_buffer, dt);
            float b_buffer[n];
            getNodeOutput(*(node.inputs+1), n, b_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i]=a_buffer[i]-b_buffer[i];
            }
            return;
            break;
        }
    case MULTIPLY:
        {
            float a_buffer[n];
            getNodeOutput(*(node.inputs), n, a_buffer, dt);
            float b_buffer[n];
            getNodeOutput(*(node.inputs+1), n, b_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i]=a_buffer[i]*b_buffer[i];
            }
            return;
            break;
        }
    case DIVIDE:
        {
            getNodeOutput(*(node.inputs), n, buffer, dt);
            float b_buffer[n];
            getNodeOutput(*(node.inputs+1), n, b_buffer, dt);
            for(int i = 0; i < n; i++){
                buffer[i]=buffer[i]/b_buffer[i];
            }
            return;
            break;
        }
    default:
        return;
    }
}

//Nodes don't copy values just the pointers

WaveNode nodeWhiteNoise(){
    WaveNode node;
    node.type = WHITE_NOISE;
    node.inputs = NULL;
    node.value = malloc(1*sizeof(pcg32_random_t));
    *((pcg32_random_t*)node.value) = make_rng(42, 63);
    return node;
}

WaveNode nodePinkNoise(){
    WaveNode node;
    node.type = PINK_NOISE;
    node.inputs = NULL;
    node.value = malloc(1*sizeof(pcg32_random_t));
    *((pcg32_random_t*)node.value) = make_rng(42, 63);
    return node;
}

WaveNode nodeBrownNoise(){
    WaveNode node;
    node.type = BROWN_NOISE;
    node.inputs = NULL;
    node.value = malloc(1*sizeof(pcg32_random_t));
    *((pcg32_random_t*)node.value) = make_rng(42, 63);
    return node;
}

WaveNode nodeNumber(float number){
    WaveNode node;
    node.type = NUMBER;
    node.inputs = NULL;
    node.value = malloc(sizeof(float));
    float* ptr = (float*)node.value;
    *ptr = number;
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

WaveNode nodeAdd(WaveNode a, WaveNode b){
    WaveNode node;
    node.type = ADD;
    node.inputs = (WaveNode*)malloc(2*sizeof(WaveNode));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.value = NULL;
    return node;
}

WaveNode nodeSub(WaveNode a, WaveNode b){
    WaveNode node;
    node.type = SUBTRACT;
    node.inputs = (WaveNode*)malloc(2*sizeof(WaveNode));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.value = NULL;
    return node;
}

WaveNode nodeMul(WaveNode a, WaveNode b){
    WaveNode node;
    node.type = MULTIPLY;
    node.inputs = (WaveNode*)malloc(2*sizeof(WaveNode));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.value = NULL;
    return node;
}

WaveNode nodeDiv(WaveNode a, WaveNode b){
    WaveNode node;
    node.type = DIVIDE;
    node.inputs = (WaveNode*)malloc(2*sizeof(WaveNode));
    node.inputs[0] = a;
    node.inputs[1] = b;
    node.value = NULL;
    return node;
}