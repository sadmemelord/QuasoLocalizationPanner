
#include "PanningWindow.h"
PanningWindow::PanningWindow(std::vector<DraggableComponent*> *draggableComponents):
    _draggableComponents(*draggableComponents)
{
    setDraggableComponentProperties();
}

PanningWindow::~PanningWindow()
{
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        _draggableComponents[channel]->removeComponentListener(this);
    }

    _draggableComponents.clear();
    _draggableComponents.shrink_to_fit();
}


//this method returns the color of the Draggable Component with a specific index inside
//the vector of Draggable Components
juce::Colour PanningWindow::getChildColour(int childIndex)
{
    auto childColour = _draggableComponents[childIndex]->getDraggableComponentColour();
    return childColour;

}

//This method is inherited from the ComponentListener class and is called whenever a Draggable Component
//is moved inside the panningWindow, its position is then mapped in relation to the Panning Window with
//X between -300 and +300 and Y between 0 and 200. These values are then used to update the component 
//variables linked to the APVTS Pan and Distance parameters
void PanningWindow::componentMovedOrResized(Component& component, bool wasMoved, bool wasResized)
{
    auto children = getChildren();

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        //checking which Draggable Component has been moved
        if (component.getName() == children[channel]->getName())
        {
            int componentPan, componentDistance;

            //mapping the X and Y to the Panning Window
            componentPan = component.getX() - this->getWidth() / 2 + component.getWidth() / 2;
            componentDistance = component.getY();
            
            //updating the Draggable Components variables linked to the APVTS
            _draggableComponents[channel]->_compX = componentPan;
            _draggableComponents[channel]->_compY = componentDistance;

            //DBG(component.getName() << " " << "X: " << componentPan << " " << "Y: " << componentDistance);
        }
    }



}
//==============================================================================
void PanningWindow::paint(juce::Graphics& g)
{
    g.setColour(CustomColors::lightGrey);
    g.drawRect(getLocalBounds(), 2.5f);
    g.setColour(CustomColors::lightGrey.withAlpha(0.25f));

    //Drawing the lines inside the Panning Window
    juce::Line<float> _verticalLine;
    juce::Line<float> _horizontalLine;
    _verticalLine.setStart(getWidth() / 2, 0.0f);
    _verticalLine.setEnd(getWidth() / 2, getBottom());
    _horizontalLine.setStart(0.0f , 3*getHeight() / 4 - getChildComponent(0)->getHeight()/4);
    _horizontalLine.setEnd(getRight(), 3*getHeight() / 4 - getChildComponent(0)->getHeight() / 4);
    g.drawLine(_verticalLine, 2.0f);
    g.drawLine(_horizontalLine, 2.0f);

    //Enabling and setting the shadow of the Draggable Components when they're dragged
    setShadowProperties(g);
}

void PanningWindow::resized()
{
    float dragCompSize = 50.f;

    //Setting the Draggable Components position based on the related APVTS parameter
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        _draggableComponents[channel]->setBounds(_draggableComponents[channel]->_compX + getWidth() / 2 - dragCompSize / 2,
                                                 _draggableComponents[channel]->_compY, dragCompSize, dragCompSize);
    }
}
