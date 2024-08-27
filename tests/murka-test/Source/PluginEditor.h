/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UIBaseComponent.h"

//==============================================================================
/**
*/
class MurkatestAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MurkatestAudioProcessorEditor (MurkatestAudioProcessor&);
    ~MurkatestAudioProcessorEditor() override;

    MurkatestAudioProcessor* processor = nullptr;
    UIBaseComponent* _UIBaseComponent = nullptr;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MurkatestAudioProcessorEditor)
};
