#ifndef RENDER_IMNODES_H
#define RENDER_IMNODES_H
#include "imnodes.h"
#include <wavegraph.h>
#include "imgui.h"

char* NODE_DESC[] = {
    "WAVETABLE", "TRIANGLE", "SAW", "SQUARE", "SIN", "OUTPUT", "ADD", "SUBTRACT", "MULTIPLY", "NUMBER", "FILTER", "COMB_FILTER", "ALLPASS_FILTER", "DIVIDE", "WHITE_NOISE", "PINK_NOISE", "BROWN_NOISE", "DELAY"
};

void render_internals(WaveNode node){
    switch(node.type){
        case NUMBER:
            ImGui::PushItemWidth(150);
            ImGui::InputFloat("Number:", (float*)node.value);
            break;
    }
}

void render_wavenode(WaveNode node){
    ImNodes::BeginNode(node.id);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text(NODE_DESC[(int)node.type]);
    ImNodes::EndNodeTitleBar();

    for(int i = 0; i < node.num_inputs; i ++){
        ImNodes::BeginInputAttribute(node.id*1024+i);
            ImGui::Text("input");
        ImNodes::EndInputAttribute();
    }

    render_internals(node);

    ImNodes::BeginOutputAttribute(node.id*1024+node.num_inputs+1);
        ImGui::Indent(40);
        ImGui::Text("output");
    ImNodes::EndOutputAttribute();
    ImNodes::EndNode();
    return;
}
#endif