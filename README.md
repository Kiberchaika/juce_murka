# JUCE Murka

## Overview

This repo is a submodule that you can add to your [JUCE](https://github.com/juce-framework/JUCE) project to use the [Murka](https://github.com/Kiberchaika/Murka)
UI library. Murka itself is added as a submodule here, so it makes sense to clone this with the `--recursive` option.

## Quickstart

Add this module to your JUCE project, then you can subclass the `murka::JuceMurkaBaseComponent` and place all your Murka code in `draw()`. 

### Like this:

```cpp
#pragma once

#include <JuceHeader.h>
#include "juce_murka/JuceMurkaBaseComponent.h"

class MainComponent : public JuceMurkaBaseComponent
{
public:
	MainComponent() {
		setSize(600, 400);
	}

	void MainComponent::initialise() override {
		JuceMurkaBaseComponent::initialise();

		// load font from Projucer's BinaryData.cpp
		m.setFontFromRawData("InterRegular.ttf", BinaryData::InterRegular_ttf, BinaryData::InterRegular_ttfSize, 12);
	}

	void draw() override {
		m.clear(100, 100, 100);
		if (m.prepare<murka::Button>({ 10, 10, 150, 45 }).text("Button 1").draw().pressed) {
			DBG("Button 1 pressed!");
		}
		if (m.prepare<murka::Button>({ 10, 60, 150, 45 }).text("Button 2").draw().pressed) {
			DBG("Button 2 pressed!");
		}
	}

private:
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

```

(c) 2024
