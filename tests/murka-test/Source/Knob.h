#pragma once

#include "MurkaView.h"
#include "MurkaBasicWidgets.h"
#include "TextField.h"

using namespace murka;

class Knob : public murka::View<Knob> {
public:
    void internalDraw(Murka & m) {
        float* data = dataToControl;

        bool isInside = isHovered() * (!editingTextNow);
        changed = false; // false unless the user changed a value using this knob
        hovered = isInside + draggingNow; // for external views to be highlighted too if needed
        bool hoveredLocal = hovered + externalHover; // shouldn't propel hoveredLocal outside so it doesn't feedback

        if (!enabled) {
            hoveredLocal = false;
        }
        
        if (enabled) {
            m.setColor(ENABLED_PARAM);
        } else {
            m.setColor(DISABLED_PARAM);
        }

        m.pushStyle();
        m.setLineWidth(4);
        m.enableFill();
        
        float width = 1.0;
        
        // "Outer" circle
        m.drawCircle(shape.size.x / 2,
                     shape.size.y * 0.35,
                     shape.size.x * (0.25 + A(0.01 * hoveredLocal)));
        
        // "Inner" circle
        m.setColor(BACKGROUND_GREY);
        m.drawCircle(shape.size.x / 2,
                     shape.size.y * 0.35,
                     shape.size.x * 0.25 - 2 * (width + A(0.5 * hoveredLocal)));
        m.setColor(BACKGROUND_GREY);
        
       // A grey colored rectangle that rotates
        m.pushMatrix();
        m.translate(shape.size.x / 2,
                    shape.size.y * 0.35,
                     0);
        
        float inputValueNormalised = ((*data - rangeFrom) / (rangeTo - rangeFrom));
        float inputValueAngleInDegrees = inputValueNormalised * 360;
        
        if (!isEndlessRotary) {
            inputValueAngleInDegrees = inputValueNormalised * 300 + 30;
        }
        
        m.rotateZRad(inputValueAngleInDegrees * (juce::MathConstants<float>::pi / 180));
        m.drawRectangle(-width * (4 + A(1 * isInside)), 0, width * (8 + A(2 * isInside)), shape.size.x * (0.25 + A(0.02 * isInside)));
        
        // A white rectangle inside a grey colored one
        m.setColor(100 + 90 * enabled + A(30 * hoveredLocal) * enabled, 255);
        float w = A(width * (1 + 1 * hoveredLocal));
        m.drawRectangle(-w, 0, w * 2, shape.size.x * 0.26);
        
        m.popMatrix();
        m.popStyle();
        
        m.setColor(100 + 90 * enabled + A(30 * hoveredLocal), 255);
        auto labelPositionY = shape.size.x * 0.8 / width + 10;
        std::string displayString = float_to_string(*data, floatingPointPrecision);

        std::function<void()> deleteTheTextField = [&]() {
            // Temporary solution to delete the TextField:
            // Searching for an id to delete the text field widget.
            // To be redone after the UI library refactoring.
            
            imIdentifier idToDelete;
            for (auto childTuple: imChildren) {
                auto childIdTuple = childTuple.first;
                if (std::get<1>(childIdTuple) == typeid(TextField).name()) {
                    idToDelete = childIdTuple;
                }
            }
            imChildren.erase(idToDelete);
        };
         
        std::string valueText = prefix + displayString + postfix;
        auto font = m.getCurrentFont();
        auto valueTextBbox = font->getStringBoundingBox(valueText, 0, 0);

        MurkaShape valueTextShape = { shape.size.x / 2 - valueTextBbox.width / 2 - 5,
                                     shape.size.x * 0.8 / width + 10,
                                     valueTextBbox.width + 10,
                                     valueTextBbox.height };
        
        if (editingTextNow) {
            auto& textFieldObject =
                m.prepare<TextField>({ valueTextShape.x() - 5, valueTextShape.y() - 5,
                    valueTextShape.width() + 10, valueTextShape.height() + 10 })
                .controlling(data)
                .withPrecision(2)
                .forcingEditorToSelectAll(shouldForceEditorToSelectAll)
                .onlyAllowNumbers(true)
                .grabKeyboardFocus()
                .draw();
            
            auto textFieldEditingFinished = textFieldObject.editingFinished;
            
            if (shouldForceEditorToSelectAll) {
                // We force selection by sending the value to text editor field
                shouldForceEditorToSelectAll = false;
            }
            
            if (!textFieldEditingFinished) {
                textFieldObject.activated = true;
                claimKeyboardFocus();
            }
            
            if (textFieldEditingFinished) {
                editingTextNow = false;
                changed = true;
                deleteTheTextField();
            }
            
        } else {
            m.prepare<murka::Label>({0, shape.size.x * 0.8 / width + 10,
                shape.size.x / width, shape.size.y * 0.5})
                .withAlignment(TEXT_CENTER).text(valueText)
                .draw();
        }
        
        bool hoveredValueText = false;
        if (valueTextShape.inside(mousePosition()) && !editingTextNow && enabled) {
            m.drawRectangle(valueTextShape.x() - 2,
                             valueTextShape.y(),
                             2,
                             2);
            m.drawRectangle(valueTextShape.x() + valueTextShape.width() + 2,
                             valueTextShape.y(),
                             2,
                             2);
            m.drawRectangle(valueTextShape.x() - 2,
                             valueTextShape.y() + valueTextShape.height(),
                             2,
                             2);
            m.drawRectangle(valueTextShape.x() + valueTextShape.width() + 2,
                             valueTextShape.y() + valueTextShape.height(),
                             2,
                             2);
            hoveredValueText = true;
        }
        
        // Action

        if ((mouseDownPressed(0)) && (!isHovered()) && (editingTextNow)) {
            // Pressed outside the knob widget while editing text. Aborting the text edit
            editingTextNow = false;
            deleteTheTextField();
        }
        
        if ((hoveredValueText) && (doubleClick()) && (enabled)) {
            editingTextNow = true;
            shouldForceEditorToSelectAll = true;
        }
        
        if ((mouseDownPressed(0)) && (isInside) && (mousePosition().y < labelPositionY) &&
            (!draggingNow) && (enabled)) {
            draggingNow = true;
            cursorHide();
        }

        if ((draggingNow) && (!mouseDown(0))) {
            draggingNow = false;
            cursorShow();
        }
        
        // Setting knob value to default if pressed alt while clicking
        bool shouldSetDefault = isKeyHeld(murka::MurkaKey::MURKA_KEY_ALT) && mouseDownPressed(0);
        
        // Don't set default by doubleclick if the mouse is in the Label/Text editor zone
        if (mousePosition().y >= labelPositionY) shouldSetDefault = false;

        if (shouldSetDefault && isInside) {
            draggingNow = false;
            *((float*)dataToControl) = defaultValue;
            cursorShow();
            changed = true;
        }
        
        if (draggingNow) {
            if (abs(mouseDelta().y) >= 1) {
                
                // Shift key fine-tune mode
                float s = speed;  // TODO: check if this speed constant should be dependent on UIScale
                if (isKeyHeld(murka::MurkaKey::MURKA_KEY_SHIFT)) {
                    s /= 10;
                }
                *((float*)dataToControl) += ( mouseDelta().y / s) * (rangeTo - rangeFrom);
            }
            
            if (*((float*)dataToControl) > rangeTo) {
                if (isEndlessRotary) {
                    *((float*)dataToControl) -= (rangeTo - rangeFrom);
                } else {
                    *((float*)dataToControl) = rangeTo;
                }
            }
            
            if (*((float*)dataToControl) < rangeFrom) {
                if (isEndlessRotary) {
                    *((float*)dataToControl) += (rangeTo - rangeFrom);
                } else {
                    *((float*)dataToControl) = rangeFrom;
                }
            }
            changed = true;
        }
    }
    
    std::stringstream converterStringStream;
    std::string float_to_string(float input, int precision) {
        converterStringStream.str(std::string());
        converterStringStream << std::fixed << std::setprecision(precision) << input;
        return (converterStringStream.str());
    }
    
    bool draggingNow = false;
    bool editingTextNow = false;
    bool shouldForceEditorToSelectAll = false;

    float rangeFrom = 0;
    float rangeTo = 1;

    std::string postfix = "";
    std::string prefix = "";
    
    float speed = 250.;
    bool enabled = true;
    bool externalHover = false; // for Pitch wheel to control the knob
    int floatingPointPrecision = 0;
    std::function<void()> cursorHide, cursorShow;
    bool isEndlessRotary = false;
    float defaultValue = 0;
    float* dataToControl = nullptr;
    bool changed = false;
    bool hovered = false;

    Knob & withParameters(float rangeFrom_,
                            float rangeTo_,
                            std::string postfix_ = "",
                            std::string prefix_ = "",
                            int floatingPointPrecision_ = 0,
                            float speed_ = 250.,
                            float defaultValue_ = 0,
                            bool isEndlessRotary_ = false,
                            bool enabled_ = true,
                            bool externalHover_ = false,
                            std::function<void()> cursrorHide_ = [](){},
                            std::function<void()> cursorShow_ = [](){}) {
//        parameterName = parameterName_;
        rangeFrom = rangeFrom_;
        rangeTo = rangeTo_;
        postfix = postfix_;
        prefix = prefix_;
        speed = speed_;
        defaultValue = defaultValue_;
        isEndlessRotary = isEndlessRotary_;
        enabled = enabled_;
        externalHover = externalHover_;
        cursorHide = cursrorHide_;
        cursorShow = cursorShow_;
        
        return *this;
    }
    
    Knob & controlling(float* dataPointer) {
        dataToControl = dataPointer;
        return *this;
    }
    
};
