#include "UIBaseComponent.h"

using namespace murka;

//==============================================================================
UIBaseComponent::UIBaseComponent(MurkatestAudioProcessor* processor_)
{
	// Make sure you set the size of the component after
    // you add any child components.
	setSize (getWidth(), getHeight());

	processor = processor_;
}

UIBaseComponent::~UIBaseComponent()
{
}

//==============================================================================
void UIBaseComponent::initialise()
{
	JuceMurkaBaseComponent::initialise();
}

void UIBaseComponent::draw()
{
    // This clears the context with our background.
    //juce::OpenGLHelpers::clear(juce::Colour(255.0, 198.0, 30.0));
    
    float scale = (float)openGLContext.getRenderingScale() * 0.7; // (Desktop::getInstance().getMainMouseSource().getScreenPosition().x / 300.0); //  0.7;

    if (scale != m.getScreenScale()) {
        m.setScreenScale(scale);
        m.updateFontsTextures(&m);
        m.clearFontsTextures();
    }

    currentMousePositionJuceScaled = m.mousePosition() / m.getScreenScale();

    m.setColor(200, 200, 200);
	m.clear();
    
    m.setLineWidth(2);
    
	// Changes the default knob reaction speed to mouse. The higher the slower.
	float knobSpeed = 250;
    
	int xOffset = 0;
	int yOffset = 40;
	int knobWidth = 70;
	int knobHeight = 87;

	// X
    auto& oneKnob = m.prepare<Knob>(MurkaShape(xOffset + 10, yOffset, knobWidth, knobHeight))
                                .controlling(&processor->one_k);
    oneKnob.rangeFrom = 1;
    oneKnob.rangeTo = 100;
    oneKnob.floatingPointPrecision = 1;
    oneKnob.speed = knobSpeed;
    oneKnob.defaultValue = 5;
    oneKnob.isEndlessRotary = false;
    oneKnob.enabled = true;
    oneKnob.cursorHide = cursorHide;
    oneKnob.cursorShow = cursorShowAndTeleportBack;
    oneKnob.draw();
    
    if (oneKnob.changed) {
		// update this parameter here, notifying host
        processor->parameterChanged(processor->paramOneKnob, processor->one_k);
	}
    
    auto& oneLabel = m.prepare<Label>(MurkaShape(xOffset + 10 + LabelOffsetX, yOffset - LabelOffsetY, knobWidth, knobHeight));
    oneLabel.label = "ONE";
    oneLabel.alignment = TEXT_CENTER;
    oneLabel.enabled = true;
    oneLabel.highlighted = oneKnob.hovered;
    oneLabel.draw();

	// Y
    auto& twoKnob = m.prepare<Knob>(MurkaShape(xOffset + 100, yOffset, knobWidth, knobHeight))
                                .controlling(&processor->two_k);
    twoKnob.rangeFrom = -1.0;
    twoKnob.rangeTo = 1.0;
    twoKnob.floatingPointPrecision = 1;
    twoKnob.speed = knobSpeed;
    twoKnob.defaultValue = 0.7;
    twoKnob.isEndlessRotary = false;
    twoKnob.enabled = true;
    twoKnob.cursorHide = cursorHide;
    twoKnob.cursorShow = cursorShowAndTeleportBack;
    twoKnob.draw();
    
    if (twoKnob.changed) {
        processor->parameterChanged(processor->paramTwoKnob, processor->two_k);
    }
    
	m.setColor(ENABLED_PARAM);
    auto& twoLabel = m.prepare<Label>(MurkaShape(xOffset + 100 + LabelOffsetX, yOffset - LabelOffsetY, knobWidth, knobHeight));
    twoLabel.label = "TWO";
    twoLabel.alignment = TEXT_CENTER;
    twoLabel.enabled = true;
    twoLabel.highlighted = twoKnob.hovered;
    twoLabel.draw();

    auto& threeKnob = m.prepare<Knob>(MurkaShape(xOffset + 190, yOffset, knobWidth, knobHeight))
                                .controlling(&processor->three_k);
    threeKnob.rangeFrom = -180;
    threeKnob.rangeTo = 180;
    threeKnob.floatingPointPrecision = 1;
    threeKnob.speed = knobSpeed;
    threeKnob.defaultValue = 0;
    threeKnob.isEndlessRotary = true;
    threeKnob.enabled = true;
    threeKnob.postfix = "º";
    threeKnob.cursorHide = cursorHide;
    threeKnob.cursorShow = cursorShowAndTeleportBack;
    threeKnob.draw();

    if (threeKnob.changed) {
        processor->parameterChanged(processor->paramThreeKnob, processor->three_k);
    }
    
    auto& threeLabel = m.prepare<Label>(MurkaShape(xOffset + 190 + LabelOffsetX, yOffset - LabelOffsetY, knobWidth, knobHeight));
    threeLabel.label = "THREE";
    threeLabel.alignment = TEXT_CENTER;
    threeLabel.enabled = true;
    threeLabel.highlighted = threeKnob.hovered;
    threeLabel.draw();
    
	/// CHECKBOXES
	float checkboxSlotHeight = 28;
    
    auto& oneCheckbox = m.prepare<Checkbox>({ xOffset + 240, yOffset + checkboxSlotHeight * 0,
                                                200, 20 })
                                                .controlling(&processor->one_c)
                                                .withLabel("ONE");
    oneCheckbox.enabled = true;
    oneCheckbox.draw();
        
    auto& twoCheckbox = m.prepare<Checkbox>({ xOffset + 240, yOffset + checkboxSlotHeight * 1,
                                                200, 20 })
                                                .controlling(&processor->two_c)
                                                .withLabel("TWO");
    twoCheckbox.enabled = true;
    twoCheckbox.draw();

    auto& threeCheckbox = m.prepare<Checkbox>({ xOffset + 240, yOffset + checkboxSlotHeight * 2,
                                                200, 20 })
                                                .controlling(&processor->three_c)
                                                .withLabel("THREE");
    threeCheckbox.enabled = (processor->three_c);
    threeCheckbox.draw();

    if (twoCheckbox.changed || threeCheckbox.changed) {
        if (twoCheckbox.checked) {
            if (threeCheckbox.checked) {
                processor->parameterChanged(processor->one_c, true);
                processor->parameterChanged(processor->two_c, true);
            } else {
                processor->parameterChanged(processor->one_c, true);
                processor->parameterChanged(processor->two_c, false);
            }
        } else {
            processor->parameterChanged(processor->one_c, false);
            processor->parameterChanged(processor->two_c, false);
        }
    }
    
    /// label
    m.setColor(200, 255);
    auto& testLabel = m.prepare<Label>(MurkaShape(m.getSize().width() - 100, m.getSize().height(), 80, 20));
    testLabel.label = "Test Label";
    testLabel.alignment = TEXT_CENTER;
    testLabel.draw();
}

//==============================================================================
void UIBaseComponent::paint (juce::Graphics& g)
{
    // You can add your component specific drawing code here!
    // This will draw over the top of the openGL background.
}

void UIBaseComponent::resized()
{
    // This is called when the UIBaseComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
