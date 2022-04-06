#pragma once

#ifdef OF_VERSION_MAJOR
#include "ofMain.h"
#else 
#include <stddef.h>
#include <string>
#include <vector>
#endif

#ifdef WIN32
#include <windows.h>
#endif

extern "C" {
#include "fontstash.h"
}

#if defined(MURKA_OF) || defined(MURKA_JUCE)
#include "murkafontstash.h"
#else 
#include "offontstash.h"
#endif

class juceFontStash {
    int font = FONS_INVALID;
    FONScontext* fs = NULL;
   
	void cleanup();

public:
    
	struct Rectangle {
		float x;
		float y;
		float width;
		float height;
	};

    juceFontStash();
    ~juceFontStash();
    
	juceFontStash& operator=(const juceFontStash& obj);
	
    void load(const std::string filename, float fontsize, bool isAbsolutePath = false, void* renderer = nullptr);
	void load(const char* data, int dataSize, float fontsize, bool isAbsolutePath = false, void* renderer = nullptr);

	void updateTexture(void* renderer = nullptr);
    void clearTexture();

	float getLineHeight();
	float stringWidth(const std::string& s);
	float stringHeight(const std::string& s);
	Rectangle getStringBoundingBox(const std::string &s, float x, float y);

	// custom function
	std::vector<Rectangle> getStringSymbolsBoundingBoxes(const std::string &s, float x, float y, bool needJoin = false);

	void drawString(const std::string &s, float x, float y);

	/// static C error handler
	static void stashError(void* uptr, int error, int val);
};
