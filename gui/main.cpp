#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imnodes.h>
#include <RtMidi.h>
#include <implot.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
extern "C"{
#include <wavegraph.h>
}
#include "render_wavenode.h"
#include <rtmidi_c.h>

MidiState midi_state;
std::vector<std::vector<WaveNode>> channels;

void midi_callback_wrapper(double timeStamp, std::vector<unsigned char> *message, void *userData){
    midi_callback(timeStamp, message->data(), message->size(), userData);
    return;
}

void drawSelectionBox(int* selected_item){
    ImGui::Begin("Add new node");
    if(ImGui::ListBox(" ", selected_item, NODE_DESC, LEN_NODE_DESC, -1));
    ImGui::End();
}

// Zeroeth midi channel might interfere
std::vector<std::vector<WaveNode>> cloneGraph(std::vector<WaveNode> graph, int num_voices){
    std::vector<std::vector<WaveNode>> voices(num_voices, std::vector<WaveNode>(graph.size()));
    for(int i = 0; i < num_voices; i++){
        for(int j = 0; j < graph.size(); j++){
            voices[i][j] = copyNode(graph[j]);
            if(voices[i][j].type==MIDI_GATE || voices[i][j].type==VELOCITY || voices[i][j].type==MIDI_PITCH){
                *(int*)(((char*)voices[i][j].value)+sizeof(MidiState*)) = i;
            }
        }
    }
}

void addNodeToTree(std::vector<WaveNode> &nodes, NodeType node_type){
    switch(node_type){
        case ADD:
            nodes.push_back(nodeAdd(-1, -1));
            break;
        case SUBTRACT:
            nodes.push_back(nodeSub(-1, -1));
            break;
        case MULTIPLY:
            nodes.push_back(nodeMul(-1, -1));
            break;
        case DIVIDE:
            nodes.push_back(nodeDiv(-1, -1));
            break;
        case SQUARE:
            nodes.push_back(nodeSqr(-1));
            break;
        case TRIANGLE:
            nodes.push_back(nodeTri(-1));
            break;
        case SAW:
            nodes.push_back(nodeSaw(-1));
            break;
        case SIN:
            nodes.push_back(nodeSin(-1));
            break;
        case POLYGON:
            nodes.push_back(nodePolygon(-1, -1));
            break;
        // Not correct yet
        case MIDI_CONTROL:
            nodes.push_back(nodeMidiControl(2, &midi_state));
            break;
        case MIDI_GATE:
            nodes.push_back(nodeMidiGate(0, &midi_state));
            break;
        case MIDI_PITCH:
            nodes.push_back(nodeMidiPitch(0, &midi_state));
            break;
        case NUMBER:
            nodes.push_back(nodeMidiPitch(0, &midi_state));
            break;
    }
}


//WaveNode DEFALUT_NODE = {.type=NUMBER, .value=&ZERO, .id=-1};

// void midicallback( double deltatime, std::vector< unsigned char > *message, void *userData )
// {
//   unsigned int nBytes = message->size();
//   for ( unsigned int i=0; i<nBytes; i++ )
//     std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
//   if ( nBytes > 0 )
//     std::cout << "stamp = " << deltatime << std::endl;
// }


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    //Setup RtMidi
    RtMidiIn* midiin;
    bool selection_active =false;
    int selected_node_to_add = -1;
    ImVec2 mouse_pos = {};

    try {
        midiin = new RtMidiIn();
    } catch (RtMidiError &error) {
        error.printMessage();
        exit( EXIT_FAILURE );
        error.printMessage();
    }

    // Check inputs.
    unsigned int nPorts = midiin->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
        portName = midiin->getPortName(i);
        }
        catch ( RtMidiError &error ) {
        error.printMessage();
        delete midiin;
        }
        std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
    }

    midiin->openPort(nPorts>1?1:0);
    // rtmidi_in_set_callback((RtMidiInPtr)midiin, midi_callback, &midi_state);
    midiin->setCallback(midi_callback_wrapper, &midi_state);
    midiin->ignoreTypes( false, false, false );

    std::vector<WaveNode> nodes;
    std::vector<std::pair<int, int>> links;
    for(int i = 0; i < 2; i++){
        nodes.push_back(nodeNumber(i+1));
    }

    nodes.push_back(nodeAdd(-1, -1));

    Wavetable sawtable = wtbl_saw(48100, 4096, 10);

    // nodes.push_back(nodeWavetable(-1, &sawtable));

    // nodes.push_back(nodeWhiteNoise());

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Enable Docking
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImNodes::CreateContext();
    ImNodes::SetNodeGridSpacePos(1, ImVec2(200.0f, 200.0f));

    int in;
    int out;
    int link_id;
    int selected_links[1024];
    float buffer[1024];

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Rendering

        ImGui::Begin("simple node editor");

        if(selection_active){
            ImGui::SetNextWindowPos(mouse_pos);
            drawSelectionBox(&selected_node_to_add);
            if(selected_node_to_add != -1){
                // nodes.push_back(nodeAdd(-1, -1));
                addNodeToTree(nodes, (NodeType)selected_node_to_add);
                selected_node_to_add = -1;
                selection_active = false;
            }
        }

        if(ImNodes::IsLinkCreated(&out, &in)){
            links.push_back({out, in});
            nodes[in/1024].inputs[in%1024] = out/1024;
        }

        ImNodes::BeginNodeEditor();

        for(int i = 0; i < links.size(); i++){
            ImNodes::Link(i, links[i].first, links[i].second);
        }

        for(auto node : nodes){
            render_wavenode(node);
        }

        ImNodes::EndNodeEditor();

        if(ImNodes::IsLinkDestroyed(&link_id)){
            links.erase(links.begin()+link_id);
        }

        if(ImGui::IsKeyPressed(ImGuiKey_X)){
            for(int i = 0; i < links.size(); i++){
                if(ImNodes::IsLinkSelected(i)){
                    std::pair<int, int> link = links[i];
                    links.erase(links.begin()+i);
                    nodes[link.second/1024].inputs[link.second%1024] = -1;
                }
            }
        }
        if(ImGui::IsKeyPressed(ImGuiKey_A)){
            selection_active = true;
            mouse_pos = ImGui::GetMousePos();
        }
        if(ImGui::IsKeyPressed(ImGuiKey_Enter)){
            selection_active = false;
        }

        if(ImGui::Shortcut(ImGuiKey_ModCtrl|ImGuiKey_Enter)){
            printf("Key comb pressed\n");
            channels = cloneGraph(nodes, NUM_VOICES);
        }

        ImGui::End();

        ImGui::Begin("Plot");

        if(ImNodes::NumSelectedNodes() == 1){
            if (ImPlot::BeginPlot("My Plot")){
                int selected_nodes[ImNodes::NumSelectedNodes()];
                ImNodes::GetSelectedNodes(selected_nodes);
                getNodeOutput(selected_nodes[0], nodes.data(), nodes.size(), 1024, buffer, 1.0/(1024));
                ImPlot::PlotLine("Node Output", buffer, 1024);
            ImPlot::EndPlot();
            }
        }

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}