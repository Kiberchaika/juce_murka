#include "MainComponent.h"

MainComponent::MainComponent()
{
    // Setup the font with proper FontOptions instead of deprecated constructor
    font = juce::Font(juce::FontOptions().withHeight(20.0f));
    
    // Create and add toggle button
    toggleButton.setButtonText("Toggle Offscreen Rendering");
    toggleButton.onClick = [this] { toggleOffscreenRendering(); };
    addAndMakeVisible(toggleButton);
    addAndMakeVisible(offscreenRenderer);

    // Set initial size
    setSize(800, 600);
}

void MainComponent::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Set font and color for text
    g.setFont(font);
    g.setColour(juce::Colours::white);

    // Display current rendering mode
    juce::String modeText = offscreenRenderer.isOffscreenRenderingEnabled() 
                          ? "Currently using OFFSCREEN rendering" 
                          : "Currently using NORMAL rendering";
    
    g.drawText(modeText, getLocalBounds().reduced(10), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // Position the toggle button at the bottom of the component
    toggleButton.setBounds(getWidth() / 2 - 100, getHeight() - 50, 200, 30);
    
    // Position the renderer component
    offscreenRenderer.setBounds(10, 10, getWidth() - 20, getHeight() - 70);
}

void MainComponent::toggleOffscreenRendering()
{
    bool newState = !offscreenRenderer.isOffscreenRenderingEnabled();
    offscreenRenderer.enableOffscreenRendering(newState);
    
    // Update the UI to reflect the new state
    repaint();
} 