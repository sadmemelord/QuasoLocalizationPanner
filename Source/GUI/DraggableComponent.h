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
    DraggableComponent(juce::RangedAudioParameter* rapX, juce::RangedAudioParameter* rapY );
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

    float _compX, _compY;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::RangedAudioParameter &_rapX;
    juce::RangedAudioParameter &_rapY;
    juce::ParameterAttachment _attachX;
    juce::ParameterAttachment _attachY;



    juce::ComponentDragger _dragger;
    juce::ComponentBoundsConstrainer _constrainer;
    juce::Label _labelName;
    juce::Font _labelFont;
    juce::Colour _colour;

};
