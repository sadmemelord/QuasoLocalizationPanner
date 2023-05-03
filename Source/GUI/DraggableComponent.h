/*
  ==============================================================================

    DraggableComponent.h
    Created: 23 Feb 2023 5:22:45pm
    Author:  Utente

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Data/CustomColors.h"

class DraggableComponent : public juce::Component
{
public:
    //==============================================================================
    DraggableComponent();
    ~DraggableComponent() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    void mouseWheelMove(const juce::MouseEvent& event,
                        const juce::MouseWheelDetails& wheel) override;

    void setDragableComponentLabel(juce::String& newName);
    void setDraggableComponentColour(juce::Colour newColour);
    juce::Colour getDraggableComponentColour();

private:
    //==============================================================================
    // Your private member variables go here...
    //juce::RangedAudioParameter& _rap;
    //juce::ParameterAttachment _attach;

    juce::ComponentDragger _dragger;
    juce::ComponentBoundsConstrainer _constrainer;
    juce::Label _labelName;
    juce::Font _labelFont;
    juce::Colour _colour;

};
