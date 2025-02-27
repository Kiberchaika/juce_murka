#pragma once

#include "JuceHeader.h"
#include "OffscreenRendererComponent.h"

class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    
private:
    // Method to toggle between rendering modes
    void toggleOffscreenRendering();
    
    // UI Components
    juce::TextButton toggleButton;
    juce::Font font;
    
    // The offscreen renderer component
    OffscreenRendererComponent offscreenRenderer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
}; 