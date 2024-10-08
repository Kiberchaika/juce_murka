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
                })

{
    parameters.addParameterListener(paramOneKnob, this);
    parameters.addParameterListener(paramTwoKnob, this);
    parameters.addParameterListener(paramThreeKnob, this);
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
juce::XmlElement* addXmlElement(juce::XmlElement& root, juce::String paramName, juce::String value)
{
    juce::XmlElement* el = root.createNewChildElement("param_" + paramName);
    el->setAttribute("value", juce::String(value));
    return el;
}

double getParameterDoubleFromXmlElement(juce::XmlElement* xml, juce::String paramName, double defVal)
{
    if (xml->getChildByName("param_" + paramName) && xml->getChildByName("param_" + paramName)->hasAttribute("value")) {
        double val = xml->getChildByName("param_" + paramName)->getDoubleAttribute("value", defVal);
        if (std::isnan(val)) {
            return defVal;
        }
        return val;
    }
    return defVal;
}

int getParameterIntFromXmlElement(juce::XmlElement* xml, juce::String paramName, int defVal)
{
    if (xml->getChildByName("param_" + paramName) && xml->getChildByName("param_" + paramName)->hasAttribute("value")) {
        return xml->getChildByName("param_" + paramName)->getDoubleAttribute("value", defVal);
    }
    return defVal;
}

void MurkatestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Store the parameters in the memory block.
    juce::MemoryOutputStream stream(destData, false);
    stream.writeString("murka_params"); // write the last major prefix
    juce::XmlElement root("Root");
    addXmlElement(root, paramOneKnob, juce::String(parameters.getParameter(paramOneKnob)->getValue()));
    addXmlElement(root, paramTwoKnob, juce::String(parameters.getParameter(paramTwoKnob)->getValue()));
    addXmlElement(root, paramThreeKnob, juce::String(parameters.getParameter(paramThreeKnob)->getValue()));    
    stream.writeString(root.toString());
}

void MurkatestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore the parameters from the memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::MemoryInputStream input(data, sizeInBytes, false);
    auto prefix = input.readString();
    juce::XmlDocument doc(input.readString());
    std::unique_ptr<juce::XmlElement> root(doc.getDocumentElement());
    
    if (!prefix.isEmpty() && (prefix == "murka_params")) {
        parameterChanged(paramOneKnob, (float)getParameterDoubleFromXmlElement(root.get(), paramOneKnob, parameters.getParameter(paramOneKnob)->getDefaultValue()));
        parameterChanged(paramTwoKnob, (float)getParameterDoubleFromXmlElement(root.get(), paramTwoKnob, parameters.getParameter(paramTwoKnob)->getDefaultValue()));
        parameterChanged(paramThreeKnob, (float)getParameterDoubleFromXmlElement(root.get(), paramThreeKnob, parameters.getParameter(paramThreeKnob)->getDefaultValue()));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MurkatestAudioProcessor();
}
