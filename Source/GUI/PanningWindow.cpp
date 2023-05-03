/*
  ==============================================================================

    PanningWindow.cpp
    Created: 19 Apr 2023 4:17:17pm
    Author:  Alberto

  ==============================================================================
*/

#include "PanningWindow.h"
PanningWindow::PanningWindow()
{
    setDraggableComponentProperties();
}

PanningWindow::~PanningWindow()
{
    _draggableComponents.clear();
    _draggableComponents.shrink_to_fit();
}



juce::Colour PanningWindow::getChildColour(int childIndex)
{
    auto childColour = _draggableComponents[childIndex]->getDraggableComponentColour();

    return childColour;

}

void PanningWindow::componentMovedOrResized(Component& component, bool wasMoved, bool wasResized)
{
    auto children = getChildren();

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        if (component.getName() == children[channel]->getName())
        {
            int componentPan, componentDistance;
            
            componentPan = component.getX() - this->getWidth() / 2 + component.getWidth() / 2;
            componentDistance = component.getY();

            DBG(component.getName() << " " << "X: " << componentPan << " " << "Y: " << componentDistance);
        }
    }



}
//==============================================================================
void PanningWindow::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour) 
    g.setColour(CustomColors::lightGrey);
    g.drawRect(getLocalBounds(), 2.5f);

    g.setColour(CustomColors::lightGrey.withAlpha(0.25f));

    //vertical lines of the panning window
    juce::Line<float> _verticalLine;
    juce::Line<float> _horizontalLine;
    _verticalLine.setStart(getWidth() / 2, 0.0f);
    _verticalLine.setEnd(getWidth() / 2, getBottom());
    _horizontalLine.setStart(0.0f , 3*getHeight() / 4 - getChildComponent(0)->getHeight()/4);
    _horizontalLine.setEnd(getRight(), 3*getHeight() / 4 - getChildComponent(0)->getHeight() / 4);
    g.drawLine(_verticalLine, 2.0f);
    g.drawLine(_horizontalLine, 2.0f);

    //shadows settings for the draggable component
    setShadowProperties(g);
}

void PanningWindow::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        _draggableComponents[channel]->centreWithSize(50, 50);
        _draggableComponents[channel]->setCentrePosition(getWidth() / 2, 3 * getHeight() / 4 - _draggableComponents[channel]->getHeight() / 4);
    }
}
