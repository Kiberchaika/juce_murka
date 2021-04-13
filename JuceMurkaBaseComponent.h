#pragma once

#include <JuceHeader.h>
#include "juce_murka.h"

namespace murka {

class JuceMurkaBaseComponent : public OpenGLAppComponent, public juce::KeyListener
{
public:
	JuceMurkaBaseComponent() {
		// Make sure you set the size of the component after
		// you add any child components.
		openGLContext.setOpenGLVersionRequired(OpenGLContext::OpenGLVersion::openGL3_2);
		openGLContext.setMultisamplingEnabled(true);

		OpenGLPixelFormat pixelFormat;
		pixelFormat.multisamplingLevel = 8;
		openGLContext.setPixelFormat(pixelFormat);

		keyAltPressed = false;
		keyCommandPressed = false;
		keyCtrlPressed = false;
		keyShiftPressed = false;
	}

	~JuceMurkaBaseComponent() {
		// This shuts down the GL system and stops the rendering calls.
		openGLContext.deactivateCurrentContext();
		openGLContext.detach();
	}

	//==============================================================================
	void initialise() override {
		getTopLevelComponent()->addKeyListener(this);

		// murka
		m.setAppComponent(this);
		m.setOpenGLContext(&openGLContext);
		m.setup();

		m.shape = { 0, 0, getWidth(), getHeight() };
		m.setupScreen();

		m.setClipboardCallbacks(
			[&]() -> std::string { return SystemClipboard::getTextFromClipboard().toStdString(); },
			[&](std::string clipboard) -> void { SystemClipboard::copyTextToClipboard(clipboard); }
		);
	}

	void shutdown() override {
		// Free any GL objects created for rendering here.
		m.closeOpenGLContext();
	}

#ifdef  WIN32
	void makeTransparent()
	{
		HWND hWnd = (HWND)getWindowHandle();
		LONG windowStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
		SetWindowLong(hWnd, GWL_EXSTYLE, windowStyle | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_COLORKEY);
	}
#else
	void makeTransparent();
#endif

	//==============================================================================
	void paint(Graphics& g) override {

	}

	void resized() override {

	}

	bool keyPressed(const juce::KeyPress &key, Component *originatingComponent) override {
		int keyCode = key.getKeyCode();
		if (keysPressed.find(keyCode) == keysPressed.end()) {
			keysPressed[keyCode] = key.getTextCharacter();
		}

		int k = murka::convertKey(keyCode);
		m.registerKeyPressed(k >= 0 ? k : keysPressed[keyCode]);

		return true;
	}

	bool keyStateChanged(bool isKeyDown, juce::Component *originatingComponent) override {
		if (juce::ModifierKeys::getCurrentModifiers().isAltDown() != keyAltPressed) {
			keyAltPressed = !keyAltPressed;
			if (keyAltPressed) {
				m.registerKeyPressed(murka::MurkaKey::MURKA_KEY_ALT);
			}
			else {
				m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_ALT);
			}
		}

		if (juce::ModifierKeys::getCurrentModifiers().isCtrlDown() != keyCtrlPressed) {
			keyCtrlPressed = !keyCtrlPressed;
			if (keyCtrlPressed) {
				m.registerKeyPressed(murka::MurkaKey::MURKA_KEY_CONTROL);
			}
			else {
				m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_CONTROL);
			}
		}

		if (juce::ModifierKeys::getCurrentModifiers().isShiftDown() != keyShiftPressed) {
			keyShiftPressed = !keyShiftPressed;
			if (keyShiftPressed) {
				m.registerKeyPressed(murka::MurkaKey::MURKA_KEY_SHIFT);
			}
			else {
				m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_SHIFT);
			}
		}

		if (juce::ModifierKeys::getCurrentModifiers().isCommandDown() != keyCommandPressed) {
			keyCommandPressed = !keyCommandPressed;
			if (keyCommandPressed) {
				m.registerKeyPressed(murka::MurkaKey::MURKA_KEY_COMMAND);
			}
			else {
				m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_COMMAND);
			}
		}

		for (auto iter = keysPressed.begin(); iter != keysPressed.end(); ++iter) {
			auto keyCode = iter->first;
			auto textCharacter = iter->second;

			if (!juce::KeyPress::isKeyCurrentlyDown(keyCode)) {
				keysPressed.erase(iter);

				int k = murka::convertKey(keyCode);
				m.registerKeyReleased(k >= 0 ? k : textCharacter);
				break;
			}
		}
		return true;
	}

	void mouseMove(const juce::MouseEvent& event) override {
		float desktopScale = openGLContext.getRenderingScale();
		m.registerMouseMoved(event.x * desktopScale, event.y * desktopScale, 0);
	}

	void mouseDown(const juce::MouseEvent& event) override {
		float desktopScale = openGLContext.getRenderingScale();
		m.registerMousePressed(event.x * desktopScale, event.y * desktopScale, 0);
	}

	void mouseUp(const juce::MouseEvent& event) override {
		float desktopScale = openGLContext.getRenderingScale();
		m.registerMouseReleased(event.x * desktopScale, event.y * desktopScale, 0);
	}

	void mouseDrag(const juce::MouseEvent& event) override {
		float desktopScale = openGLContext.getRenderingScale();
		m.registerMouseDragged(event.x * desktopScale, event.y * desktopScale, 0);
	}

protected:
	Murka m;

private:
	std::map<int, juce::juce_wchar> keysPressed;
	bool keyAltPressed;
	bool keyCommandPressed;
	bool keyCtrlPressed;
	bool keyShiftPressed;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuceMurkaBaseComponent)
};



};
