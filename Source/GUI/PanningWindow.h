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
#define INPUTCHANNELS 4

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

    juce::Colour getChildColour(int childIndex);

    void setDraggableComponentProperties();
    void setShadowProperties(juce::Graphics& g);

private:

    std::vector<DraggableComponent*> &_draggableComponents;

    //==============================================================================
    // Your private member variables go here...



    
};
