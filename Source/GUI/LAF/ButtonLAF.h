
#pragma once
#include <JuceHeader.h>
#include "../Data/CustomColors.h"

class ButtonLAF : public juce::LookAndFeel_V4
{
public:


    ButtonLAF();
    ~ButtonLAF() override;


    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool isButtonDown) override;


    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override;


private:

};