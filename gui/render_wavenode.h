#ifndef RENDER_IMNODES_H
#define RENDER_IMNODES_H
#include "imnodes.h"
#include <wavegraph.h>
#include <filter.h>
#include <envelope.h>
#include <map>
#include "imgui.h"

char* NODE_DESC[] = {
    "ADSR",
    "MIDI_GATE", 
    "MIDI_PITCH",
    "MODWHEEL",
    "PITCHBEND",
    "VELOCITY", 
    "WAVETABLE",
    "TRIANGLE", 
    "SAW", 
    "SQUARE", 
    "SIN", 
    "POLYGON", 
    "OUTPUT", 
    "ADD", 
    "SUBTRACT", 
    "MULTIPLY", 
    "NUMBER", 
    "FILTER_LOWPASS", 
    "FILTER_HIGHPASS",
    "FILTER_BANDPASS", 
    "FILTER_NOTCH", 
    "FILTER_APF", 
    "FILTER_PEAKEQ", 
    "COMB_FILTER", 
    "ALLPASS_FILTER", 
    "DIVIDE", 
    "WHITE_NOISE", 
    "PINK_NOISE", 
    "BROWN_NOISE", 
    "DELAY", 
    "DISTORTION", 
    "BITCRUSHER", 
    "MIDI_CONTROL",
    "REVERB"
};

int LEN_NODE_DESC = 34;

std::map<int, int> in_attribute_map; // Input attribute to node
std::map<int, int> out_attribute_map; // Output attribute to node

void render_internals(WaveNode node){
    switch(node.type){
        case NUMBER:
        case PINK_NOISE:
        case WHITE_NOISE:
        case BROWN_NOISE:
            ImGui::PushItemWidth(150);
            ImGui::InputFloat("Number:", ((float*)node.value));
            break;
        case MIDI_CONTROL:
            ImGui::PushItemWidth(150);
            ImGui::InputInt("Input channel id", (int*)(((char*)node.value)+sizeof(MidiState*)), 1, 2);
        case ADSR:
            ImGui::PushItemWidth(150);
            ImGui::InputFloat("Ab", &((ADSREnvelope*)node.value)->ab);
            ImGui::PushItemWidth(150);
            ImGui::InputFloat("Ac", &((ADSREnvelope*)node.value)->ac);
            ImGui::PushItemWidth(150);
            ImGui::InputFloat("Db", &((ADSREnvelope*)node.value)->db);
            ImGui::PushItemWidth(150);
            ImGui::InputFloat("Dc", &((ADSREnvelope*)node.value)->dc);
            ImGui::PushItemWidth(150);
            ImGui::InputFloat("Rb", &((ADSREnvelope*)node.value)->rb);
            ImGui::PushItemWidth(150);
            ImGui::InputFloat("Rc", &((ADSREnvelope*)node.value)->rc);
        // case FILTER_HIGHPASS:
        // case FILTER_LOWPASS:
        // case FILTER_BANDPASS:
        // case FILTER_APF:
        //     ImGui::PushItemWidth(150);
        //     ImGui::InputFloat("Number:", ((Filter*)node.value)->);
        default:
            break;
    }
}

void render_wavenode(WaveNode node){
    ImNodes::BeginNode(node.id);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text(NODE_DESC[node.type]);
    ImNodes::EndNodeTitleBar();

    for(int i = 0; i < node.num_inputs; i ++){
        in_attribute_map[node.id*1024+i] = node.id;
        ImNodes::BeginInputAttribute(node.id*1024+i);
            ImGui::Text("input");
        ImNodes::EndInputAttribute();
    }

    render_internals(node);
    out_attribute_map[node.id*1024+node.num_inputs+1] = node.id;
    ImNodes::BeginOutputAttribute(node.id*1024+node.num_inputs+1);
        ImGui::Indent(40);
        ImGui::Text("output");
    ImNodes::EndOutputAttribute();
    ImNodes::EndNode();
    return;
}
#endif