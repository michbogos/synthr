#ifndef OPENGL_COMPONENT
#define OPENGL_COMPONENT

#include<JuceHeader.h>


class SynthrGUI final : public juce::AudioAppComponent{
    public:
        SynthrGUI(){
            return;
        }

        ~SynthrGUI(){
            return;
        }
        void newOpenGLContextCreated();
        void renderOpenGL();
        void openGLContextClosing();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthrGUI)
};
#endif
