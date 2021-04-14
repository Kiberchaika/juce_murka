#include <AppKit/NSOpenGL.h>
#include <AppKit/NSWindow.h>
#include "JuceMurkaBaseComponent.h"

#if defined(JUCE_APP_VERSION)
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
                sendKeyReleasedToSubPlugin(k >= 0 ? k : KeyPress(key.first).getTextCharacter());
            }
        
        // Releasing modifier keys

        if (keyShiftPressed) {
            sendKeyReleasedToSubPlugin(ofKey::OF_KEY_SHIFT);
            keyShiftPressed = false;
        }

        if (keyCtrlPressed) {
            sendKeyReleasedToSubPlugin(ofKey::OF_KEY_CONTROL);
            keyCtrlPressed = false;
        }

        if (keyAltPressed) {
            sendKeyReleasedToSubPlugin(ofKey::OF_KEY_ALT);
            keyAltPressed = false;
        }

        if (keyCommandPressed) {
            sendKeyReleasedToSubPlugin(ofKey::OF_KEY_COMMAND);
            keyCommandPressed = false;
        }
    }
}

void JuceMurkaBaseComponent::removeKeyboardMonitor()
{
    [NSEvent removeMonitor:keyboardMonitor];
}

void JuceMurkaBaseComponent::addKeyboardMonitor()
{
    keyboardMonitor = [NSEvent addLocalMonitorForEventsMatchingMask: NSEventMaskFlagsChanged|NSEventMaskKeyDown|NSEventMaskKeyUp handler:^NSEvent*(NSEvent* event){
        
        // TODO: isWindowFocused sometimes doesn't work the first time you switch to this window.
        // Supposedly this only happens in AudioPluginHost due to a bug in it. Should investigate in other hosts.
        // For now, I'm removing the check for window focus, therefore the key presses will be sent to all windows.
        
//        if (!isWindowFocused) { return event; }
        
        // First, let's check if any of the modifier keys were pressed
        if (event.type == NSEventTypeFlagsChanged) {

            // Shift key
        
            if ((event.keyCode == 56) || (event.keyCode == 60)) { // this is Shift key, left and right
                if ([event modifierFlags] & NSEventModifierFlagShift) {
                    sendKeyPressedToSubplugin(ofKey::OF_KEY_SHIFT);
                    keyShiftPressed = true;
                    return event;
                } else {
                    sendKeyReleasedToSubPlugin(ofKey::OF_KEY_SHIFT);
                    keyShiftPressed = false;
                    return event;
                }
            }

            // Control key

            if ((event.keyCode == 59) || (event.keyCode == 62)) { // this is Control key, left and right
                if ([event modifierFlags] & NSEventModifierFlagControl) {
                    sendKeyPressedToSubplugin(ofKey::OF_KEY_CONTROL);
                    keyCtrlPressed = true;
                    return event;
                } else {
                    sendKeyReleasedToSubPlugin(ofKey::OF_KEY_CONTROL);
                    keyCtrlPressed = false;
                    return event;
                }
            }

            // Option key

            if ((event.keyCode == 58) || (event.keyCode == 61)) { // this is Option key, left and right
                if ([event modifierFlags] & NSEventModifierFlagOption) {
                    sendKeyPressedToSubplugin(ofKey::OF_KEY_ALT);
                    keyAltPressed = true;
                    return event;
                } else {
                    sendKeyReleasedToSubPlugin(ofKey::OF_KEY_ALT);
                    keyAltPressed = false;
                    return event;
                }
            }
            
            // Command key

            if ((event.keyCode == 55) || (event.keyCode == 54)) { // this is Command key, left and right
                if ([event modifierFlags] & NSEventModifierFlagCommand) {
                    sendKeyPressedToSubplugin(ofKey::OF_KEY_COMMAND);
                    keyCommandPressed = true;
                    return event;
                } else {
                    sendKeyReleasedToSubPlugin(ofKey::OF_KEY_COMMAND);
                    keyCommandPressed = false;
                    return event;
                }
            }
            
        }
        
        
        if (event.type == NSEventTypeKeyDown) {
            auto convertedKey = convertKey(event.keyCode);
            if (convertedKey == -1) convertedKey = [event.characters characterAtIndex:0];
            
            auto subpluginResult = sendKeyPressedToSubplugin(convertedKey);
            keysPressed[convertedKey] = KeyPress(convertedKey).getTextCharacter();

            if (subpluginResult) {
                return nil;
            } else {
                return event;
            }
        }

        if (event.type == NSEventTypeKeyUp) {
            auto convertedKey = convertKey(event.keyCode);
            if (convertedKey == -1) convertedKey = [event.characters characterAtIndex:0];
            
            auto subpluginResult = sendKeyReleasedToSubPlugin(convertedKey);
            keysPressed.erase(convertedKey);
            
            if (subpluginResult) {
                return nil;
            } else {
                return event;
            }

        }
         

        // To avoid error message, return nil
        return nil;
        }
     ];
    
    /*
    id monitor=[NSEvent addLocalMonitorForEventsMatchingMask:..
    // remove monitor
    [NSEvent removeMonitor:monitor];
     */
    
}
#endif
