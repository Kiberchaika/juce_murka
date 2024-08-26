#pragma once

#include "MurkaView.h"
#include "MurkaBasicWidgets.h"

using namespace murka;

class Label : public murka::View<Label> {
public:
    void internalDraw(Murka & m) {
        
        bool isInside = inside();
        //* !areInteractiveChildrenHovered(c) *
        //hasMouseFocus(m);

		auto font = m.getCurrentFont();
		if (customFont) {
			font = font;
		}

		MurkaColor bgColor = backgroundColor;
		m.enableFill();
		if (bgColor.getAlpha() != 0.0) {
			m.setColor(bgColor);
			if (alignment == TEXT_LEFT) {
				m.drawRectangle(0, 0, font->getStringBoundingBox(label, 0, 0).width + 10, shape.size.y);
			}
		}

		// color 
		MurkaColor fgColor = customColor ? color : m.getColor();
		float anim = enabled ? 1.0 * 40 / 255 * A(highlighted) : 0.0;
		fgColor.setRed(fgColor.getRed() + anim - fgColor.getRed() * 0.5 * !enabled);
		fgColor.setGreen(fgColor.getGreen() + anim - fgColor.getGreen() * 0.5 * !enabled);
		fgColor.setBlue(fgColor.getBlue() + anim - fgColor.getBlue() * 0.5 * !enabled);

		m.setColor(fgColor);
		if (alignment == TEXT_LEFT) {
			font->drawString(label, 5, 0);
		}
		if (alignment == TEXT_CENTER) {
			float textX = 5 + (shape.size.x - 10) / 2 - font->getStringBoundingBox(label, 0, 0).width / 2;
			font->drawString(label, textX, 0);
		}
		if (alignment == TEXT_RIGHT) {
			float textX = (shape.size.x - 10) - font->getStringBoundingBox(label, 0, 0).width;
			font->drawString(label, textX, 0);
		}

		// Testing vertical centering
			//context.renderer->setColor(255);
			//ofDrawLine(0, 0, context.getSize().x, 0);
			//context.renderer->setColor(255, 0, 0);
			//ofDrawLine(0, context.getSize().y / 2, context.getSize().x, context.getSize().y / 2);
			//context.renderer->setColor(255);
			//ofDrawLine(0, context.getSize().y, context.getSize().x, context.getSize().y);
	};

	// Here go parameters and any parameter convenience constructors. You need to define something called Parameters, even if it's NULL.
//    std::string label;
    TextAlignment alignment = TEXT_LEFT;
    
    Label& withTextAlignment(TextAlignment a) {
        alignment = a;
        return *this;
    }

    Label& withText(std::string text) {
        label = text;
        return *this;
    }

    MurkaColor color = { 0.98, 0.98, 0.98 };
    MurkaColor backgroundColor = { 0., 0., 0., 0. };

    FontObject* font;

    bool customColor = false;
    bool customFont = false;
    bool enabled = true;
    bool highlighted = false;

    MURKA_PARAMETER(Label, // class name
                    std::string, // parameter type
                    label, // parameter variable name
                    text, // setter
                    "" // default
    )

	// The results type, you also need to define it even if it's nothing.
	bool result;

	virtual bool wantsClicks() override { return false; } // override this if you want to signal that you don't want clicks
};
