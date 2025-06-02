#include <JuceHeader.h>
#include <juce_opengl/juce_opengl.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_juce/imgui_impl_juce.h>

class SynthrGui : public juce::Component, public juce::OpenGLRenderer{
    private:
        juce::OpenGLContext glctx;
        ImGuiContext *imguictx;
    public:
        SynthrGui(){
            glctx.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
            glctx.setRenderer(this);
            glctx.attachTo(*this);
            glctx.setContinuousRepainting(true);
            return;
        }
        ~SynthrGui(){
            return;
        }

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

class Application    : public JUCEApplication
{
public:
    //==============================================================================
    Application() {}

    const String getApplicationName() override       { return "OpenGL3DAppTemplate"; }
    const String getApplicationVersion() override    { return "1.0.0"; }

    void initialise (const String&) override         { mainWindow.reset (new MainWindow ("OpenGL3DAppTemplate", new SynthrGui, *this)); }
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