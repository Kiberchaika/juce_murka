#include "juce_fontStash.h"

#define FONTSTASH_IMPLEMENTATION
extern "C" {
#include "fontstash.h"
}

#if defined(MURKA_OF) || defined(MURKA_JUCE)
#define MURKAFONTSTASH_IMPLEMENTATION
#include "murkafontstash.h"
#else 
#define OFFONTSTASH_IMPLEMENTATION
#include "offontstash.h"
#endif

#define ATLAS_MAX_SIZE 1024

void juceFontStash::cleanup() {
	if (fs != NULL) {
		glfontDelete(fs);
		fs = NULL;
	}
}

juceFontStash::juceFontStash(){
	fs = NULL;
}

juceFontStash::~juceFontStash(){
	cleanup();
}

juceFontStash & juceFontStash::operator=(const juceFontStash & obj) {
	if (this != &obj) {
		cleanup();
		font = obj.font;
		fs = obj.fs;
	}
	return *this;
}

bool juceFontStash::load(const std::string filename, float fontsize, bool isAbsolutePath, void* renderer) {
	bool bUseArb = false;

	cleanup();

	fs = glfontCreate(512, 512, FONT_ZERO_TOPLEFT);
	if (fs == NULL) {
		printf("Could not create stash.\n");
		return false;
	}

#if defined(MURKA_OF) || defined(MURKA_JUCE)
	MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
	context->renderer = (murka::MurkaRendererBase*)renderer;
#endif

	std::string path = filename;
	font = fontAddFont(fs, "font", path.c_str());
	if (font == FONT_INVALID) {
		printf("Error loading font (might be a wrong filename): %s\n", path.c_str());
		return false;
	}

	fontClearState(fs);
	fontSetFont(fs, font);

#if defined(MURKA_OF) || defined(MURKA_JUCE)
	float scale = context->renderer->getScreenScale();
	fontSetSize(fs, scale * fontsize);
#else
	fontSetSize(fs, fontsize);
#endif

	fontSetAlign(fs, FONT_ALIGN_LEFT | FONT_ALIGN_TOP);

	//fontSetColor(fs, white);
	//fontSetSpacing(fs, 5.0f);
	//fontSetBlur(fs, 10.0f);

	return true;
}

bool juceFontStash::load(const char* data, int dataSize, float fontsize, bool isAbsolutePath, void* renderer) {
	bool bUseArb = false;

	cleanup();

	fs = glfontCreate(512, 512, FONT_ZERO_TOPLEFT);
	if (fs == NULL) {
		printf("Could not create stash.\n");
		return false;
	}

#if defined(MURKA_OF) || defined(MURKA_JUCE)
	MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
	context->renderer = (murka::MurkaRendererBase*)renderer;
#endif

	font = fontAddFontMem(fs, "font", (unsigned char*)data, dataSize, 1);
	if (font == FONT_INVALID) {
		printf("Error loading font (might be a wrong data\n");
		return false;
	}


	fontClearState(fs);

#if defined(MURKA_OF) || defined(MURKA_JUCE)
	float scale = context->renderer->getScreenScale();
	fontSetSize(fs, scale * fontsize);
#else
	fontSetSize(fs, fontsize);
#endif

	fontSetAlign(fs, FONT_ALIGN_LEFT | FONT_ALIGN_TOP);

	//fontSetColor(fs, white);
	//fontSetSpacing(fs, 5.0f);
	//fontSetBlur(fs, 10.0f);
	
	return true;
}

void juceFontStash::updateTexture(void* renderer) {
    if (fs) {
#if defined(MURKA_OF) || defined(MURKA_JUCE)
        MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
        context->renderer = (murka::MurkaRendererBase*)renderer;

        bool bUseArb = context->renderer->getUsingArbTex();
		context->renderer->disableArbTex();

        fontResetAtlas(fs, fs->params.width, fs->params.height);
        
        if (context->img != 0) {
            context->img->update();
        }
        
        if (context->vbo != 0) {
            context->vbo->update(GL_STREAM_DRAW, ((murka::MurkaRenderer*)context->renderer)->getMainShaderAttribLocation("position"), ((murka::MurkaRenderer*)context->renderer)->getMainShaderAttribLocation("uv"), ((murka::MurkaRenderer*)context->renderer)->getMainShaderAttribLocation("col"));
        }
        
        bUseArb ? context->renderer->enableArbTex() : context->renderer->disableArbTex();
#endif
    }
}

void juceFontStash::clearTexture() {
    if (fs) {
#if defined(MURKA_OF) || defined(MURKA_JUCE)
        MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
        
        if (context->img != 0) {
            context->img->clearTexture();
        }
        
        if (context->vbo != 0) {
            context->vbo->clear();
        }
#endif
    }
}

float juceFontStash::getLineHeight() {
	float lh = 0;
	if (font != FONT_INVALID) {
		fontVertMetrics(fs, NULL, NULL, &lh);
#if defined(MURKA_OF) || defined(MURKA_JUCE)
		MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
		lh /= context->renderer->getScreenScale();
#endif
	}
	return lh;
}

 float juceFontStash::stringWidth(const std::string & s) {
	if (font != FONT_INVALID) {
		float width = fontTextBounds(fs, 0, 0, s.c_str(), NULL, NULL);
#if defined(MURKA_OF) || defined(MURKA_JUCE)
		MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
		width /= context->renderer->getScreenScale();
#endif
		return width;
	}
	return 0;
}

 float juceFontStash::stringHeight(const std::string & s) {
	 float bounds[4] = { 0, 0, 0, 0 };
	 if (font != FONT_INVALID) {
		 float height = fontTextBounds(fs, 0, 0, s.c_str(), NULL, bounds);
#if defined(MURKA_OF) || defined(MURKA_JUCE)
		 MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
		 height /= context->renderer->getScreenScale();
#endif
		 return height;
	 }
	 return 0; 
 }

 juceFontStash::Rectangle juceFontStash::getStringBoundingBox(const std::string & s, float x, float y) {
	 float bounds[4] = { 0, 0, 0, 0 };
	 if (font != FONT_INVALID) {
		 fontTextBounds(fs, x, y, s.c_str(), NULL, bounds);
	 }

#if defined(MURKA_OF) || defined(MURKA_JUCE)
	 MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
	 float scale = context->renderer->getScreenScale();
	 return juceFontStash::Rectangle{ x + bounds[0] / scale, y + bounds[1] / scale, bounds[2] / scale, bounds[3] / scale };
#else
	 return juceFontStash::Rectangle{ bounds[0], bounds[1], bounds[2], bounds[3] };
#endif
 }

 
 std::vector<juceFontStash::Rectangle> juceFontStash::getStringSymbolsBoundingBoxes(const std::string & s, float x, float y, bool needJoin) {
	 std::vector<juceFontStash::Rectangle> rects;
	 if (font != FONT_INVALID) {

		 FONTstate* state = font__getState(fs);
		 unsigned int codepoint;
		 unsigned int utf8state = 0;
		 FONTquad q;
		 FONTglyph* glyph = NULL;
		 int prevGlyphIndex = -1;
		 short isize = (short)(state->size*10.0f);
		 short iblur = (short)state->blur;
		 float scale;
		 FONTfont* font;
		 float startx, advance;
		 float minx, miny, maxx, maxy;

		 if (fs == NULL) return rects;
		 if (state->font < 0 || state->font >= fs->nfonts) return rects;
		 font = fs->fonts[state->font];
		 if (font->data == NULL) return rects;

#if defined(MURKA_OF) || defined(MURKA_JUCE)
		 MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
		 isize /= context->renderer->getScreenScale();
#endif

		 scale = font__tt_getPixelHeightScale(&font->font, (float)isize / 10.0f);

		 // Align vertically.
		 y += font__getVertAlign(fs, font, state->align, isize);

		 minx = maxx = x;
		 miny = maxy = y;
		 startx = x;

		 char* str = (char*)s.data();
		 char* end = str + strlen(str);

		 for (; str != end; ++str) {
			 if (font__decutf8(&utf8state, &codepoint, *(const unsigned char*)str))
				 continue;
			 glyph = font__getGlyph(fs, font, codepoint, isize, iblur);
			 if (glyph != NULL) {
				 font__getQuad(fs, font, prevGlyphIndex, glyph, scale, state->spacing, &x, &y, &q);
				 if (q.x0 < minx) minx = q.x0;
				 if (q.x1 > maxx) maxx = q.x1;
				 if (fs->params.flags & FONT_ZERO_TOPLEFT) {
					 if (q.y0 < miny) miny = q.y0;
					 if (q.y1 > maxy) maxy = q.y1;
				 }
				 else {
					 if (q.y1 < miny) miny = q.y1;
					 if (q.y0 > maxy) maxy = q.y0;
				 }
				 rects.push_back(juceFontStash::Rectangle{ q.x0, q.y0, q.x1 - q.x0, q.y1 - q.y0 });
			 }
			 prevGlyphIndex = glyph != NULL ? glyph->index : -1;
		 }

		 advance = x - startx;

		 // Align horizontally
		 if (state->align & FONT_ALIGN_LEFT) {
			 // empty
		 }
		 else if (state->align & FONT_ALIGN_RIGHT) {
			 minx -= advance;
			 maxx -= advance;
			 for (size_t i = 0; i < rects.size(); i++) {
				 rects[i].x -= advance;
			 }
		 }
		 else if (state->align & FONT_ALIGN_CENTER) {
			 minx -= advance * 0.5f;
			 maxx -= advance * 0.5f;
			 for (size_t i = 0; i < rects.size(); i++) {
				 rects[i].x -= advance * 0.5f;
			 }
		 }

		 if (needJoin) {
			 for (size_t i = 0; i < rects.size(); i++) {
				 if (i < rects.size() - 1) {
					 rects[i].width = rects[i + 1].x - rects[i].x;
				 }
				 rects[i].y = miny;
				 rects[i].height = maxy - miny;
			 }
		 }
	 }


	 return rects;
 }
 

void juceFontStash::drawString(const std::string & s, float x, float y) {
	if (font != FONT_INVALID) {
#if defined(MURKA_OF) || defined(MURKA_JUCE)
		MURKAFONTcontext* context = (MURKAFONTcontext*)fs->params.userPtr;
		float scale = context->renderer->getScreenScale();
		fontDrawText(fs, x * scale, y * scale, s.c_str(), NULL);
#else
		fontDrawText(fs, x, y, s.c_str(), NULL);
#endif
	}
}

void juceFontStash::stashError(void* ptr, int error, int val) {
	FONTcontext* context = (FONTcontext*)ptr;
	switch (error) {
	case FONT_ATLAS_FULL: {
		std::cout << "font atlas texture full, expanding";
		int width, height;
		fontGetAtlasSize(context, &width, &height);
		if (width <= ATLAS_MAX_SIZE && height <= ATLAS_MAX_SIZE) {
			fontExpandAtlas(context, width * 2, height * 2);
		}
		else {
			std::cout << "couldn't expand atlas more than " << ATLAS_MAX_SIZE;
		}
		break;
	}
	case FONT_SCRATCH_FULL:
		std::cout << "scratch full, tried to allocate " << val << " has " << FONT_SCRATCH_BUF_SIZE;
		break;
	case FONT_STATES_OVERFLOW:
		std::cout << "state overflow";
		break;
	case FONT_STATES_UNDERFLOW:
		std::cout << "state underflow";
		break;
	}
}
