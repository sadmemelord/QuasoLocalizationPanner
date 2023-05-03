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
    PanningWindow();
    ~PanningWindow() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void componentMovedOrResized(Component& component, bool wasMoved, bool wasResized) override;

    juce::Colour getChildColour(int childIndex);

    void setDraggableComponentProperties();
    void setShadowProperties(juce::Graphics& g);

private:
    //==============================================================================
    // Your private member variables go here...

    DraggableComponent _dragComp1;
    DraggableComponent _dragComp2;
    DraggableComponent _dragComp3;
    DraggableComponent _dragComp4;

    std::vector<DraggableComponent*> _draggableComponents{ &_dragComp1, &_dragComp2, &_dragComp3, &_dragComp4 };
    
};
