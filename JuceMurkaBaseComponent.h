#pragma once

#include <JuceHeader.h>
#include "juce_murka/juce_murka.h"
#include "juce_opengl/juce_opengl.h"

#ifdef __APPLE__
#include <objc/objc.h>
#endif

namespace murka {

	
class JuceMurkaBaseComponent : public juce::OpenGLAppComponent, public juce::KeyListener
{
public:
	JuceMurkaBaseComponent() {
		// Make sure you set the size of the component after
		// you add any child components.
		openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::OpenGLVersion::openGL3_2);
		openGLContext.setMultisamplingEnabled(true);

		juce::OpenGLPixelFormat pixelFormat;
		pixelFormat.multisamplingLevel = 8;
		openGLContext.setPixelFormat(pixelFormat);

        // Giving Murka a way to call this component's grab and release keyboard focus functions so that
        // the Juce is notified that Murka asks for keyboard events.
        m.setKeyboardFocusRequestCallbacks([&]() {
			juce::MessageManager::callAsync([&]() {
				this->grabKeyboardFocus();
				});
        },
                                           [&]() {
            juce::MessageManager::callAsync([&]() { 
                this->giveAwayKeyboardFocus();
            });
			
        } );
	}

	virtual ~JuceMurkaBaseComponent() {
		shutdownOpenGL();
	}

	//==============================================================================
	void initialise() override {
        addKeyListener(this);
        setWantsKeyboardFocus(true);

		// murka
		m.setAppComponent(this);
		m.setOpenGLContext(&openGLContext);
		m.setup();

		m.shape = { 0, 0, getWidth(), getHeight() };
		m.setupScreen();

		m.setClipboardCallbacks(
			[&]() -> std::string { return juce::SystemClipboard::getTextFromClipboard().toStdString(); },
			[&](std::string clipboard) -> void { juce::SystemClipboard::copyTextToClipboard(clipboard); }
		);
	}

	void render()
	{ 
		checkModifierKeys();

		if (scale != (float)openGLContext.getRenderingScale()) {
			scale = (float)openGLContext.getRenderingScale();
			m.setScreenScale(scale);
			m.updateFontsTextures(&m);
			m.clearFontsTextures();
		}

		m.startFrame();
		m.begin();
		draw();
		m.end();
	}

	virtual void draw() = 0;

	void shutdown() override {
		// Free any GL objects created for rendering here.
		m.closeOpenGLContext();
	}

#ifdef WIN32
	void makeTransparent()
	{
		HWND hWnd = (HWND)getWindowHandle();
		LONG windowStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
		SetWindowLong(hWnd, GWL_EXSTYLE, windowStyle | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_COLORKEY);
	}
#else
	void makeTransparent();

    bool isWindowFocused = false;
    
    void checkMainWindow();
    
#endif

    int convertKey(int key) {
        if (key == juce::KeyPress::F1Key) return MurkaKey::MURKA_KEY_F1;
        if (key == juce::KeyPress::F2Key) return MurkaKey::MURKA_KEY_F2;
        if (key == juce::KeyPress::F3Key) return MurkaKey::MURKA_KEY_F3;
        if (key == juce::KeyPress::F4Key) return MurkaKey::MURKA_KEY_F4;
        if (key == juce::KeyPress::F5Key) return MurkaKey::MURKA_KEY_F5;
        if (key == juce::KeyPress::F6Key) return MurkaKey::MURKA_KEY_F6;
        if (key == juce::KeyPress::F7Key) return MurkaKey::MURKA_KEY_F7;
        if (key == juce::KeyPress::F8Key) return MurkaKey::MURKA_KEY_F8;
        if (key == juce::KeyPress::F9Key) return MurkaKey::MURKA_KEY_F9;
        if (key == juce::KeyPress::F10Key) return MurkaKey::MURKA_KEY_F10;
        if (key == juce::KeyPress::F11Key) return MurkaKey::MURKA_KEY_F11;
        if (key == juce::KeyPress::F12Key) return MurkaKey::MURKA_KEY_F12;
        if (key == juce::KeyPress::leftKey) return MurkaKey::MURKA_KEY_LEFT;
        if (key == juce::KeyPress::rightKey) return MurkaKey::MURKA_KEY_RIGHT;
        if (key == juce::KeyPress::upKey) return MurkaKey::MURKA_KEY_UP;
        if (key == juce::KeyPress::downKey) return MurkaKey::MURKA_KEY_DOWN;
        if (key == juce::KeyPress::homeKey) return MurkaKey::MURKA_KEY_HOME;
        if (key == juce::KeyPress::endKey) return MurkaKey::MURKA_KEY_END;
        if (key == juce::KeyPress::deleteKey) return MurkaKey::MURKA_KEY_DEL;
        if (key == juce::KeyPress::spaceKey) return MurkaKey::MURKA_KEY_SPACE;
        if (key == juce::KeyPress::backspaceKey) return MurkaKey::MURKA_KEY_BACKSPACE;
        if (key == juce::KeyPress::escapeKey) return MurkaKey::MURKA_KEY_ESC;
        if (key == juce::KeyPress::tabKey) return MurkaKey::MURKA_KEY_TAB;
		if (key == juce::KeyPress::returnKey) return MurkaKey::MURKA_KEY_RETURN;

        return -1;
    }
    
	//==============================================================================
	void paint(juce::Graphics& g) override {

	}

	void resized() override {

	}

	bool keyPressed(const juce::KeyPress &key, Component *originatingComponent) override {
		int keyCode = key.getKeyCode();
		if (keysPressed.find(keyCode) == keysPressed.end()) {
			keysPressed[keyCode] = key.getTextCharacter();
		}

		int k = convertKey(keyCode);
		m.registerKeyPressed(k >= 0 ? k : keysPressed[keyCode]);

        if (!m.isPlugin) return true;
		return m.doesHaveAWidgetThatHoldsKeyboardFocus();
	}

	void checkModifierKeys() {
		if (juce::ModifierKeys::getCurrentModifiers().isAltDown() != altKeyPressed) {
			altKeyPressed = juce::ModifierKeys::getCurrentModifiers().isAltDown();
			if (altKeyPressed) {
				m.registerKeyPressed(murka::MurkaKey::MURKA_KEY_ALT);
			}
			else {
				m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_ALT);
			}
		}

		if (juce::ModifierKeys::getCurrentModifiers().isCtrlDown() != ctrlKeyPressed) {
			ctrlKeyPressed = juce::ModifierKeys::getCurrentModifiers().isCtrlDown();
			if (ctrlKeyPressed) {
				m.registerKeyPressed(murka::MurkaKey::MURKA_KEY_CONTROL);
			}
			else {
				m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_CONTROL);
			}
		}

		if (juce::ModifierKeys::getCurrentModifiers().isShiftDown() != shiftKeyPressed) {
			shiftKeyPressed = juce::ModifierKeys::getCurrentModifiers().isShiftDown();
			if (shiftKeyPressed) {
				m.registerKeyPressed(murka::MurkaKey::MURKA_KEY_SHIFT);
			}
			else {
				m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_SHIFT);
			}
		}

		if (juce::ModifierKeys::getCurrentModifiers().isCommandDown() != commandKeyPressed) {
			commandKeyPressed = juce::ModifierKeys::getCurrentModifiers().isCommandDown();
			if (commandKeyPressed) {
				m.registerKeyPressed(murka::MurkaKey::MURKA_KEY_COMMAND);
			}
			else {
				m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_COMMAND);
			}
		}
	}

	bool keyStateChanged(bool isKeyDown, juce::Component *originatingComponent) override {
		for (auto iter = keysPressed.begin(); iter != keysPressed.end(); ++iter) {
			auto keyCode = iter->first;
			auto textCharacter = iter->second;

			if (!juce::KeyPress::isKeyCurrentlyDown(keyCode)) {
				keysPressed.erase(iter);

				int k = convertKey(keyCode);
				m.registerKeyReleased(k >= 0 ? k : textCharacter);
				break;
			}
		}
		return m.doesHaveAWidgetThatHoldsKeyboardFocus();
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

	void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheelDetails) override {
		float desktopScale = openGLContext.getRenderingScale();
		m.registerMouseScrolled(wheelDetails.deltaX * desktopScale, wheelDetails.deltaY * desktopScale);
	}

protected:
	Murka m;

private:
	std::map<int, juce::juce_wchar> keysPressed;
	bool altKeyPressed = false;
	bool commandKeyPressed = false;
	bool ctrlKeyPressed = false;
	bool shiftKeyPressed = false;
	float scale = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuceMurkaBaseComponent)
};

};
