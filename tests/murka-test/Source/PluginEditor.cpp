/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MurkatestAudioProcessorEditor::MurkatestAudioProcessorEditor (MurkatestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);

    _UIBaseComponent = new UIBaseComponent(&p);
    _UIBaseComponent->setSize(getWidth(), getHeight());
    addAndMakeVisible(_UIBaseComponent);

}

MurkatestAudioProcessorEditor::~MurkatestAudioProcessorEditor()
{
    _UIBaseComponent->shutdownOpenGL();
    removeAllChildren();
    delete _UIBaseComponent;
}

//==============================================================================
void MurkatestAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (40, 40, 40));
    g.setColour (juce::Colour (40, 40, 40));
    g.setFont (juce::FontOptions (12.0f));
}

void MurkatestAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
