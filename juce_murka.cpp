#include "juce_murka.h"
#include "juce_fontStash.cpp"

namespace murka {

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
    if (key == juce::KeyPress::backspaceKey) return MurkaKey::MURKA_KEY_BACKSPACE;
    if (key == juce::KeyPress::escapeKey) return MurkaKey::MURKA_KEY_ESC;
    if (key == juce::KeyPress::tabKey) return MurkaKey::MURKA_KEY_TAB;

	return -1;
}

}
