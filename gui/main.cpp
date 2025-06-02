#include <JuceHeader.h>
#include <juce_opengl/juce_opengl.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_juce/imgui_impl_juce.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include <juce_audio_processors/juce_audio_processors.h>


class SynthrGui :  public juce::AudioProcessorEditor, public juce::OpenGLRenderer{
    private:
        juce::OpenGLContext glctx;
        ImGuiContext *imguictx;
    public:
        AudioPluginAudioProcessor &ap;
        SynthrGui(AudioPluginAudioProcessor &p): AudioProcessorEditor(&p), ap(p){
            glctx.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
            glctx.setRenderer(this);
            glctx.attachTo(*this);
            glctx.setContinuousRepainting(true);
            return;
        }
        ~SynthrGui(){
            return;
        }

        void paint(juce::Graphics &) override {}
        void resized() override {}

        // void initialise() override{
        //     // imguictx = ImGui::CreateContext();
        //     // ImGui_ImplJuce_Init(*this, glctx);
        //     // ImGui_ImplOpenGL3_Init();
        //     return;
        // }

        void newOpenGLContextCreated() override{
            imguictx = ImGui::CreateContext();
            ImGui_ImplJuce_Init(*this, glctx);
            ImGui_ImplOpenGL3_Init();
            return;
        }

        // void render() override{
        //     // ImGui::SetCurrentContext(imguictx);
        //     // ImGui_ImplOpenGL3_NewFrame();
        //     // ImGui_ImplJuce_NewFrame();
        //     // ImGui::NewFrame();

        //     // glClearColor(0, 0, 0, 1);
        //     // glClear(GL_COLOR_BUFFER_BIT);

        //     // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //     return;
        // }

        void renderOpenGL() override{
            using namespace juce::gl;
            ImGui::SetCurrentContext(imguictx);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplJuce_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Window A");
            ImGui::Text("This is window A");
            ImGui::End();

            ImGui::Render();

            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            return;
        }

        // void shutdown() override{
        //     return;
        // }

        void openGLContextClosing() override{
            return;
        }
};

class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor(){
        return;
    }
    ~AudioPluginAudioProcessor() override{
        return;
    };

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override{
        return;
    };
    void releaseResources() override{
        return;
    };

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override{
        return false;
    };

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override{
        return;
    };
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override{
        return new SynthrGui(*this);
    };
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override{
        return "Synthr";
    };

    bool acceptsMidi() const override {return true;};
    bool producesMidi() const override {return true;};
    bool isMidiEffect() const override {return true;};
    double getTailLengthSeconds() const override {return 0;};

    //==============================================================================
    int getNumPrograms() override {return 1;};
    int getCurrentProgram() override {return 0;};
    void setCurrentProgram (int index) override {return;};
    const juce::String getProgramName (int index) override {return "Synthr";};
    void changeProgramName (int index, const juce::String& newName) override {};

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override {};
    void setStateInformation (const void* data, int sizeInBytes) override {};

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};

//==============================================================================
class Application    : public JUCEApplication
{
public:
    //==============================================================================
    Application() {}
    AudioPluginAudioProcessor apap;

    const String getApplicationName() override       { return "OpenGL3DAppTemplate"; }
    const String getApplicationVersion() override    { return "1.0.0"; }

    void initialise (const String&) override         { mainWindow.reset (new MainWindow ("OpenGL3DAppTemplate", new SynthrGui(apap), *this)); }
    void shutdown() override                         { mainWindow = nullptr; }

private:
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (const String& name, Component* c, JUCEApplication& a)
            : DocumentWindow (name, Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons),
              app (a)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (c, true);

           #if JUCE_ANDROID || JUCE_IOS
            setFullScreen (true);
           #else
            setResizable (true, false);
            setResizeLimits (300, 250, 10000, 10000);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            app.systemRequestedQuit();
        }

    private:
        JUCEApplication& app;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
START_JUCE_APPLICATION (Application)

juce::AudioProcessor * JUCE_CALLTYPE createPluginFilter()
{
  return new AudioPluginAudioProcessor;
}

// class SynthrProcessor final : public juce::AudioProcessor{
//     public:
//         SynthrProcessor(){
//             return;
//         }
//         ~SynthrProcessor(){
//             return;
//         }
//     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthrProcessor)
// };

// class SynthrEditor final : public juce::AudioProcessorEditor{
//     public:
//         explicit SynthrEditor(SynthrEditor&);
//         ~SynthrEditor(){
//             return;
//         }
//     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthrEditor)
// };

// juce::AudioProcessor JUCE_CALLTYPE createPluginFilter()
// {
//   return new Plugin();
// }