#ifndef RENDER_IMNODES_H
#define RENDER_IMNODES_H
#include "imnodes.h"
#include <wavegraph.h>
#include <map>
#include "imgui.h"

char* NODE_DESC[] = {
    "WAVETABLE", "TRIANGLE", "SAW", "SQUARE", "SIN", "OUTPUT", "ADD", "SUBTRACT", "MULTIPLY", "NUMBER", "FILTER", "COMB_FILTER", "ALLPASS_FILTER", "DIVIDE", "WHITE_NOISE", "PINK_NOISE", "BROWN_NOISE", "DELAY"
};

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
        default:
            break;
    }
}

void render_wavenode(WaveNode node){
    ImNodes::BeginNode(node.id);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text(NODE_DESC[(int)node.type]);
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