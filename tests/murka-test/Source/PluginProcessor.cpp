/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::String MurkatestAudioProcessor::paramOneKnob("one_k");
juce::String MurkatestAudioProcessor::paramTwoKnob("two_k");
juce::String MurkatestAudioProcessor::paramThreeKnob("three_k");
juce::String MurkatestAudioProcessor::paramOneCheckbox("one_c");
juce::String MurkatestAudioProcessor::paramTwoCheckbox("two_c");
juce::String MurkatestAudioProcessor::paramThreeCheckbox("three_c");

//==============================================================================
MurkatestAudioProcessor::MurkatestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters(*this, &mUndoManager, juce::Identifier("murka-test"),
               {
                    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(paramOneKnob, 1),
                                                            TRANS("One_Knob"),
                                                            juce::NormalisableRange<float>(-180.0f, 180.0f, 0.01f), one_k, "", juce::AudioProcessorParameter::genericParameter,
                                                            [](float v, int) { return juce::String (v, 1) + "°"; },
                                                            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),
                    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(paramTwoKnob, 1),
                                                            TRANS("Two_Knob"),
                                                            juce::NormalisableRange<float>(-90.0f, 90.0f, 0.01f), two_k, "", juce::AudioProcessorParameter::genericParameter,
                                                            [](float v, int) { return juce::String (v, 1) + "°"; },
                                                            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),
                    std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(paramThreeKnob, 1),
                                                            TRANS("Three_Knob"),
                                                            juce::NormalisableRange<float>(-100.0f, 100.0f, 0.01f), three_k, "", juce::AudioProcessorParameter::genericParameter,
                                                            [](float v, int) { return juce::String (v, 1); },
                                                            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),
                    std::make_unique<juce::AudioParameterBool>(juce::ParameterID(paramOneCheckbox, 1), TRANS("One_Checkbox"), one_c),
                    std::make_unique<juce::AudioParameterBool>(juce::ParameterID(paramTwoCheckbox, 1), TRANS("Two_Checkbox"), two_c),
                    std::make_unique<juce::AudioParameterBool>(juce::ParameterID(paramThreeCheckbox, 1), TRANS("Three_Checkbox"), three_c),
                })

{
    parameters.addParameterListener(paramOneKnob, this);
    parameters.addParameterListener(paramTwoKnob, this);
    parameters.addParameterListener(paramThreeKnob, this);
    parameters.addParameterListener(paramOneCheckbox, this);
    parameters.addParameterListener(paramTwoCheckbox, this);
    parameters.addParameterListener(paramThreeCheckbox, this);
}

MurkatestAudioProcessor::~MurkatestAudioProcessor()
{
}

//==============================================================================
const juce::String MurkatestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MurkatestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MurkatestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MurkatestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MurkatestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MurkatestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MurkatestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MurkatestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MurkatestAudioProcessor::getProgramName (int index)
{
    return {};
}

void MurkatestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MurkatestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MurkatestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MurkatestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MurkatestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

void MurkatestAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == paramOneKnob) {
        one_k = newValue; // update pannerSettings value from host
        parameters.getParameter(paramOneKnob)->setValue(newValue);
    } else if (parameterID == paramTwoKnob) {
        two_k = newValue; // update pannerSettings value from host
        parameters.getParameter(paramTwoKnob)->setValue(newValue);
    } else if (parameterID == paramThreeKnob) {
        three_k = newValue; // update pannerSettings value from host
        parameters.getParameter(paramThreeKnob)->setValue(newValue);
    } else if (parameterID == paramOneCheckbox) {
        one_c = newValue; // update pannerSettings value from host
        parameters.getParameter(paramOneCheckbox)->setValue(newValue);
    } else if (parameterID == paramTwoCheckbox) {
        two_c = newValue; // update pannerSettings value from host
        parameters.getParameter(paramTwoCheckbox)->setValue(newValue);
    } else if (parameterID == paramThreeCheckbox) {
        three_c = newValue; // update pannerSettings value from host
        parameters.getParameter(paramThreeCheckbox)->setValue(newValue);
    }
}

//==============================================================================
bool MurkatestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MurkatestAudioProcessor::createEditor()
{
    return new MurkatestAudioProcessorEditor (*this);
}

//==============================================================================
void MurkatestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MurkatestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
 
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MurkatestAudioProcessor();
}
