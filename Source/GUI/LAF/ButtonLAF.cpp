/*
  ==============================================================================

    ButtonLAF.cpp
    Created: 27 Apr 2023 3:53:41pm
    Author:  Alberto

  ==============================================================================
*/

#include "ButtonLAF.h"

ButtonLAF::ButtonLAF()
{

}

ButtonLAF::~ButtonLAF()
{

}

void ButtonLAF::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                                     bool shouldDrawButtonAsHighlighted, bool isButtonDown)
{
    juce::Colour buttonFillColour;

    if (button.getToggleState())
        buttonFillColour = button.findColour(juce::TextButton::ColourIds::buttonColourId);
    else
        buttonFillColour = CustomColors::lightGrey;


    auto buttonArea = button.getLocalBounds();
    float edge = 5.0f;
    buttonArea.removeFromLeft(edge);
    buttonArea.removeFromTop(edge);

    //Button shadow
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(buttonArea.toFloat(), 5.0f);

    auto offset = button.getToggleState() ? -2 : -edge;
    buttonArea.translate(offset, offset);

    g.setColour(buttonFillColour);
    g.fillRoundedRectangle(buttonArea.toFloat(), 5.0f);
}

void ButtonLAF::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown)
{
    auto font = getTextButtonFont(button, button.getHeight());
    font.setBold(true);
    g.setFont(font);
    g.setColour(juce::Colours::black);

    auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
    auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;

    auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
    auto leftIndent = 0.5; //juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2)); 
    auto rightIndent = 3; //juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    auto textWidth = button.getWidth() - leftIndent - rightIndent;

    auto edge = 5.0f;
    auto offset = button.getToggleState() ? 2 : 0;

    if (textWidth > 0)
        g.drawFittedText(button.getButtonText(),
            leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge,
            juce::Justification::centred, 2);
}