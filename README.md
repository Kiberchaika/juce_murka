# JUCE Murka
![](https://raw.githubusercontent.com/Kiberchaika/Murka/master/murka_logo.jpg?raw=true)

Murka is a lightweight UI library aimed at a modern, rich, GPU-accelerated and highly interactive audio plugin UI design. It supports JUCE 7 but it's not dependent on it.

Inspired by ImGUI, it's an "immediate mode" UI. It means that it doesn't require you to design an explicit hierarchy, you write your draw and interaction code in a single draw thread, without managing the widgets lifecycle or input thread interaction with the draw thread.

Murka is designed to make it as easy as possible to create custom widgets with real-time feedback. It requires some knowledge of OpenGL to use it to the fullest but provides drawing functions for fonts and basic shapes.

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

### TODO
- Test and resolve linux target and uncomment the [linux tests](https://github.com/Kiberchaika/juce_murka/blob/main/.github/workflows/test.yaml#L31-L34)

(c) 2024
