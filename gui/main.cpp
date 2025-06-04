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
#include <fft.h>
#include <wavegraph.h>
}
#include "render_wavenode.h"
#include <rtmidi_c.h>
#include <soundio/soundio.h>
#define __LINUX_ALSA__

MidiState midi_state;
std::vector<std::vector<WaveNode>> channels;

int selected_node = -1;
bool locked = true;

static void soundio_callback(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max) {
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    float float_sample_rate = outstream->sample_rate;
    float seconds_per_frame = 1.0f / float_sample_rate;
    struct SoundIoChannelArea *areas;
    int frames_left = frame_count_max;
    int err;

    while (frames_left > 0) {
        int frame_count = frames_left;

        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
            fprintf(stderr, "%s\n", soundio_strerror(err));
            exit(1);
        }

        if (!frame_count)
            break;
        
        float samples[frame_count];
        for(int i = 0; i < frame_count; i++){
            samples[i] = 0;
        }
        if(selected_node > -1 && !locked){
            float tmp[frame_count];
            for(int voice = 0; voice < NUM_VOICES; voice++){
                getNodeOutput(selected_node, channels[voice].data(), channels[voice].size(), frame_count, tmp, 1.0f/float_sample_rate);
                for(int i = 0; i < frame_count; i++){
                    samples[i] += tmp[i];
                }
            }
        }

        for (int frame = 0; frame < frame_count; frame += 1) {
            for (int channel = 0; channel < layout->channel_count; channel += 1) {
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                *ptr = samples[frame];
            }
        }
        if ((err = soundio_outstream_end_write(outstream))) {
            fprintf(stderr, "%s\n", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
    }
}

void midi_callback_wrapper(double timeStamp, std::vector<unsigned char> *message, void *userData){
    midi_callback(timeStamp, message->data(), message->size(), userData);
    return;
}

void drawSelectionBox(int* selected_item){
    ImGui::Begin("Add new node");
    ImGui::PushItemWidth(-1);
    if(ImGui::ListBox(" ", selected_item, NODE_DESC, LEN_NODE_DESC, 15));
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
            if(voices[i][j].type==TRIANGLE || voices[i][j].type==SIN || voices[i][j].type==WAVETABLE || voices[i][j].type==SAW || voices[i][j].type==SQUARE || voices[i][j].type==POLYGON){
                *((float*)voices[i][j].value) = 0.0f;
            }
            if(voices[i][j].type==FILTER_LOWPASS || voices[i][j].type==FILTER_HIGHPASS || voices[i][j].type==FILTER_BANDPASS || voices[i][j].type==FILTER_APF){
                ((Biquad*)voices[i][j].value)->out1 = 0;
                ((Biquad*)voices[i][j].value)->out2 = 0;
                ((Biquad*)voices[i][j].value)->out3 = 0;
            }
            // if(voices[i][j].type==DELAY){
            //     memcpy(voices[i][j].value);
            // }
        }
    }
    return voices;
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
            nodes.push_back(nodeNumber(0));
            break;
        case FILTER_LOWPASS:
            nodes.push_back(nodeFilterLowpass(-1, -1, -1));
            break;
        case FILTER_HIGHPASS:
            nodes.push_back(nodeFilterHighpass(-1, -1, -1));
            break; 
        case FILTER_BANDPASS:
            nodes.push_back(nodeFilterBandpass(-1, -1, -1));
            break;
        case FILTER_APF:
            nodes.push_back(nodeFilterAPF(-1, -1, -1));
            break;
        case WHITE_NOISE:
            nodes.push_back(nodeWhiteNoise());
            break;
        case BROWN_NOISE:
            nodes.push_back(nodeBrownNoise());
            break;
        case BITCRUSHER:
            nodes.push_back(nodeBitcrusher(-1, -1));
            break;
        case DISTORTION:
            nodes.push_back(nodeDistortion(-1));
            break;
        case ADSR:
        {
            ADSREnvelope adsr;
            nodes.push_back(nodeAdsr(-1, &adsr));
        }
        case REVERB:
            nodes.push_back(nodeReverb(-1));
            break;
        case DELAY:
            nodes.push_back(nodeDelay(-1, 10000, 1.0));
            break;
    }
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    //Setup Soundio
    int err;
    struct SoundIo *soundio = soundio_create();
    if (!soundio) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    if ((err = soundio_connect(soundio))) {
        fprintf(stderr, "error connecting: %s", soundio_strerror(err));
        return 1;
    }

    soundio_flush_events(soundio);

    int default_out_device_index = soundio_default_output_device_index(soundio);
    printf("%d\n", soundio_output_device_count(soundio));
    if (default_out_device_index < 0) {
        fprintf(stderr, "no output device found");
        return 1;
    }

    struct SoundIoDevice *device = soundio_get_output_device(soundio, default_out_device_index);
    if (!device) {
        fprintf(stderr, "out of memory");
        return 1;
    }

    fprintf(stderr, "Output device: %s\n", device->name);

    struct SoundIoOutStream *outstream = soundio_outstream_create(device);
    outstream->software_latency = 0.05;
    outstream->format = SoundIoFormatFloat32NE;
    outstream->write_callback = soundio_callback;
    //outstream->underflow_callback = NULL;

    if ((err = soundio_outstream_open(outstream))) {
        fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
        return 1;
    }

    if (outstream->layout_error)
        fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

    if ((err = soundio_outstream_start(outstream))) {
        fprintf(stderr, "unable to start device: %s", soundio_strerror(err));
        return 1;
    }


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

    nodes.push_back(nodeWavetable(-1, &sawtable));

    // nodes.push_back(nodeWhiteNoise());

    // nodes[4].value_len = 16;

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
    // ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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

    auto editor_context = ImNodes::CreateContext();
    auto context = ImNodes::EditorContextCreate();
    ImNodes::SetNodeGridSpacePos(1, ImVec2(200.0f, 200.0f));
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodesIO imnodes_controls = ImNodes::GetIO();

    int in;
    int out;
    int link_id;
    int started_at;
    int ended_at;
    int selected_links[1024];
    float buffer[1024];
    float buffer2[1024];

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

        ImGui::Begin("Node Editor");

        if(selection_active){
            ImGui::SetNextWindowPos(mouse_pos);
            drawSelectionBox(&selected_node_to_add);
            if(selected_node_to_add != -1){
                // nodes.push_back(nodeAdd(-1, -1));
                addNodeToTree(nodes, (NodeType)selected_node_to_add);
                selected_node_to_add = -1;
                selection_active = false;
                locked = true;
                channels = cloneGraph(nodes, NUM_VOICES);
                locked = false;
            }
        }

        if(ImNodes::IsLinkCreated(&out, &in)){
            links.push_back({out, in});
            nodes[in/1024].inputs[in%1024] = out/1024;
            channels = cloneGraph(nodes, NUM_VOICES);
            locked = false;
        }
        ImNodes::EditorContextSet(context);
        ImNodes::BeginNodeEditor();

        for(int i = 0; i < links.size(); i++){
            ImNodes::Link(i, links[i].first, links[i].second);
        }

        for(auto node : nodes){
            render_wavenode(node);
        }

        ImNodes::EndNodeEditor();

        if(ImNodes::IsLinkDestroyed(&link_id)){
            auto link = links[link_id];
            links.erase(links.begin()+link_id);
            nodes[link.second/1024].inputs[link.second%1024] = -1;
            channels = cloneGraph(nodes, NUM_VOICES);
        }

        // if(ImNodes::IsLinkDropped(&started_at, true)){
        //     for(int i = 0; i < links.size(); i++){
        //         if(links[i] == std::pair<int, int>(started_at, ended_at)){
        //             links.erase(links.begin()+i);
        //             nodes[links[i].second/1024].inputs[links[i].second%1024] = -1;
        //         }
        //     }
        // }

        if(ImGui::IsKeyPressed(ImGuiKey_X)){
            for(int i = 0; i < links.size(); i++){
                if(ImNodes::IsLinkSelected(i)){
                    std::pair<int, int> link = links[i];
                    links.erase(links.begin()+i);
                    nodes[link.second/1024].inputs[link.second%1024] = -1;
                }
            }
            for(int i = 0; i < nodes.size(); i++){
                if(ImNodes::IsNodeSelected(i)){
                    nodes.erase(nodes.begin()+i);
                    for(int j = 0; j < links.size(); j++){
                        if(nodes[links[j].second/1024].inputs[links[j].second%1024] == i){
                            links.erase(links.begin()+j);
                            nodes[links[j].second/1024].inputs[links[j].second%1024] = -1;
                        }
                    }
                }
            }
            cloneGraph(nodes, NUM_VOICES);
        }
        if(ImGui::IsKeyPressed(ImGuiKey_A)){
            selection_active = true;
            mouse_pos = ImGui::GetMousePos();
        }
        if(ImGui::IsKeyPressed(ImGuiKey_Enter)){
            selection_active = false;
        }

        if(ImGui::IsKeyPressed(ImGuiKey_LeftCtrl)){
            // printf("Key comb pressed\n");
            channels = cloneGraph(nodes, NUM_VOICES);
            locked = false;
        }

        ImGui::End();

        ImGui::Begin("Plot");

        // if(ImNodes::NumSelectedNodes() == 1){
        //     int selected_nodes[ImNodes::NumSelectedNodes()];
        //     ImNodes::GetSelectedNodes(selected_nodes);
        //     selected_node = selected_nodes[0];
        // }

        if(ImNodes::NumSelectedNodes() == 1){
            if (ImPlot::BeginPlot("Time Domain")){
                int selected_nodes[ImNodes::NumSelectedNodes()];
                ImNodes::GetSelectedNodes(selected_nodes);
                selected_node = selected_nodes[0];
                //getNodeOutput(selected_nodes[0], nodes.data(), nodes.size(), 1024, buffer, 1.0/(1024));
                //ImPlot::PlotLine("Node Output", buffer, 1024);
            ImPlot::EndPlot();
            }
            // if (ImPlot::BeginPlot("Frequency Domain")){
            //     fftf(buffer, 1024, -1);
            //     for(int i = 0; i < 1024; i++){
            //         buffer[i] *= 1024;
            //     }
            //     ImPlot::PlotLine("Node Output", buffer, 1024);
            // ImPlot::EndPlot();
            // }
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