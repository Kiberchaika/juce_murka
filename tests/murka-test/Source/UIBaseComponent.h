#pragma once

#include <JuceHeader.h>

#include "juce_murka/JuceMurkaBaseComponent.h"

#include "PluginProcessor.h"

#include "Label.h"
#include "Knob.h"
#include "Checkbox.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class UIBaseComponent : public JuceMurkaBaseComponent
{
    MurkatestAudioProcessor* processor = nullptr;

public:
    //==============================================================================
    UIBaseComponent(MurkatestAudioProcessor* processor);
	~UIBaseComponent();

    //==============================================================================
    void initialise() override;
    void draw() override;
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
	MurkaPoint cachedMousePositionWhenMouseWasHidden = { 0, 0 };
	MurkaPoint currentMousePositionJuceScaled = { 0, 0 };

    // date and time stamp
    juce::Time now = juce::Time::getCurrentTime();
    juce::String nowString = juce::String::formatted(
        "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        now.getYear(), now.getMonth() + 1, now.getDayOfMonth(),
        now.getHours(), now.getMinutes(), now.getSeconds(), now.getMilliseconds()
    );
    
    std::function<void()> cursorHide = [&]() {
		setMouseCursor(juce::MouseCursor::NoCursor);
		cachedMousePositionWhenMouseWasHidden = currentMousePositionJuceScaled;
	};
	std::function<void()> cursorShow = [&]() {
		setMouseCursor(juce::MouseCursor::NormalCursor);
	};
	std::function<void()> cursorShowAndTeleportBack = [&]() {
		setMouseCursor(juce::MouseCursor::NormalCursor);
        juce::Desktop::setMousePosition(localPointToGlobal(juce::Point<int>((int)cachedMousePositionWhenMouseWasHidden.x, (int)cachedMousePositionWhenMouseWasHidden.y)));
	};
	std::function<void(int, int)> teleportCursor = [&](int x, int y) {
		//
	};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIBaseComponent)
};
