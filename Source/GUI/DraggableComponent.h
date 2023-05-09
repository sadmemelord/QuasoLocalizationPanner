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
    //==============================================================================
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    //==============================================================================
    void setDragableComponentLabel(juce::String& newName);
    juce::Label* getDragableComponentLabel();
    void setDraggableComponentColour(juce::Colour newColour);
    juce::Colour getDraggableComponentColour();
    //==============================================================================
    //Public data members are probably not correct for safety issues and these can be
    //replace with Get and Set methods. They represent the component X and Y coordinates
    //inside the Panning Window
    float _compX;
    float _compY;

private:
    //Ranged Audio Parameters (Pan and Distance) are passed to each Draggable Component constructor by reference
    juce::RangedAudioParameter &_rapX;
    juce::RangedAudioParameter &_rapY;

    //To attach the UI values to the APVTS parameters an Attachment class is needed 
    juce::ParameterAttachment _attachX;
    juce::ParameterAttachment _attachY;

    //With an instance of ComponentDragger the whole component can be dragged around inside its parent's bound.
    //A ComponentBoundsConstrainer object is needed to restrict the dragger position
    juce::ComponentDragger _dragger;
    juce::ComponentBoundsConstrainer _constrainer;

    //Each DraggableComponent comes with a label associated to the track channel it represents and a colour
    juce::Label _labelName;
    juce::Colour _colour;

};
