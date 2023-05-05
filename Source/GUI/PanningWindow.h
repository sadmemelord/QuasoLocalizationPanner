/*
  ==============================================================================

    PanningWindow.h
    Created: 19 Apr 2023 4:17:17pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "DraggableComponent.h"
#include "Data/CustomColors.h"
//#define INPUTCHANNELS 4

class PanningWindow : public juce::Component, juce::ComponentListener
{
public:
    //==============================================================================
    PanningWindow(std::vector<DraggableComponent*>  *draggableComponents);
    ~PanningWindow() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void componentMovedOrResized(Component& component, bool wasMoved, bool wasResized) override;

    //==============================================================================
    juce::Colour getChildColour(int childIndex);
    void setDraggableComponentProperties();
    void setShadowProperties(juce::Graphics& g);

private:

    //The draggable components that represent each track can be moved inside the panning window's bounds.
    //Because these components are attached to the Distance and Pan parameters of the apvts they're constructed
    //in the Editor class and they're passed as a vector of pointer to the panningWindow class
    std::vector<DraggableComponent*> &_draggableComponents;

    //==============================================================================
    // Your private member variables go here...



    
};
