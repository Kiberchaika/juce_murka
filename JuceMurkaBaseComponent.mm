#include <juce_opengl/juce_opengl.h>
#include <AppKit/NSOpenGL.h>
#include <AppKit/NSWindow.h>

#include "JuceMurkaBaseComponent.h"

using namespace murka;

void JuceMurkaBaseComponent::makeTransparent()
{
	NSView* handle = (NSView*)getWindowHandle();
	NSWindow* window = [handle window];
	[window setAcceptsMouseMovedEvents: YES];
	[window setIgnoresMouseEvents:NO];
    [window setOpaque:NO];

/*	
	[window setLevel:NSMainMenuWindowLevel + 1];
    [window setAlphaValue: 1.0];
	[window setHasShadow: NO];
	[window setMovableByWindowBackground: NO];
	[window setCanBecomeKeyWindow: YES];
	[window setAcceptsMouseMovedEvents: YES];

	setOpaque(false);
*/

	NSOpenGLContext* context = (NSOpenGLContext*)openGLContext.getRawContext();
	GLint aValue = 0;
	[context setValues : &aValue forParameter : NSOpenGLCPSurfaceOpacity];
}

void JuceMurkaBaseComponent::checkMainWindow()
{
    bool windowWasFocused = isWindowFocused;
    
    NSView* handle = (NSView*)getWindowHandle();
    if(handle) {
        NSWindow* window = [handle window];
        isWindowFocused = [window isMainWindow];
    } else {
        isWindowFocused = false;
    }
    
    
    if (windowWasFocused && !isWindowFocused) {
        // Just unfocused the window, let's send the key releases.
        // Releasing normal keys
        
        for (auto &key: keysPressed) {
                keysPressed.erase(key.first);

                int k = convertKey(key.first);
                m.registerKeyReleased(k >= 0 ? k : juce::KeyPress(key.first).getTextCharacter());
            }
        
        // Releasing modifier keys

        if (shiftKeyPressed) {
            m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_SHIFT);
            shiftKeyPressed = false;
        }

        if (ctrlKeyPressed) {
            m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_CONTROL);
            ctrlKeyPressed = false;
        }

        if (altKeyPressed) {
            m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_ALT);
            altKeyPressed = false;
        }

        if (commandKeyPressed) {
            m.registerKeyReleased(murka::MurkaKey::MURKA_KEY_COMMAND);
            commandKeyPressed = false;
        }
    }
}



