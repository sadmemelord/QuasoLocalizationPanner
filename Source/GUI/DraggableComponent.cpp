
#include "DraggableComponent.h"

DraggableComponent::DraggableComponent(juce::RangedAudioParameter* rapX, juce::RangedAudioParameter* rapY) :
    _rapX(*rapX), //passing the Pan parameter
    _rapY(*rapY), //passing the Distance parameter
    _attachX(_rapX, [this](float) {repaint(); }), //Lambda callback function to repaint the UI with the new parameters
    _attachY(_rapY, [this](float) {repaint(); }), 
    _compX(juce::jmap(rapX->getValue(),-300.f,300.f)), //Position inside the Panning Window based on the parameter values
    _compY(juce::jmap(rapY->getValue(), 0.f, 200.f))
{

    //Setting label properties
    addAndMakeVisible(_labelName);
    _labelName.setEditable(false, true, false);
    _labelName.setColour(juce::Label::textColourId, juce::Colours::black);

    _constrainer.setMinimumOnscreenAmounts(50, 50, 50, 50);

    //This method updates the UI with the current APVTS parameters
    _attachX.sendInitialUpdate();
    _attachY.sendInitialUpdate();
}

DraggableComponent::~DraggableComponent()
{

}

//==============================================================================
void DraggableComponent::paint(juce::Graphics& g)
{
    //The Draggable Component is just a rounded rectangle
    g.setColour(_colour);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 5.0f);
}

void DraggableComponent::resized()
{
    //The label is centered inside the Draggable Component
    float labelHeight = 20;
    _labelName.setBounds(0, getHeight() / 2 - labelHeight / 2, getWidth(), labelHeight);
}

void DraggableComponent::mouseDown(const juce::MouseEvent& event)
{
    //Routine to drag the component called when the mouse is clicked 
    _dragger.startDraggingComponent(this, event);

    //When a component is dragged its parameters need to change according to the position
    _attachX.beginGesture();
    _attachY.beginGesture();
}
void DraggableComponent::mouseUp(const juce::MouseEvent& event)
{
    _labelName.setVisible(true);

    //When the component is released there's no need to continuosly update the parameters
    _attachX.endGesture();
    _attachY.endGesture();
}
void DraggableComponent::mouseDrag(const juce::MouseEvent& event)
{
    _dragger.dragComponent(this, event, &_constrainer);
    _labelName.setVisible(false);

    //The Pan is represented between -1 and +1 and the Distance between 0 and 1.
    //The component X and Y coordinates are transformed to represent those parameters
    _attachX.setValueAsPartOfGesture((double)(_compX / 300.f));
    _attachY.setValueAsPartOfGesture((double)(_compY / 200.f));
}

void DraggableComponent::mouseDoubleClick(const juce::MouseEvent& event)
{
    centreWithSize(50, 50);
    setCentrePosition(getParentWidth() / 2, 3 * getParentHeight() / 4 - this->getHeight() / 4);

    //When the component is double clicked it's position and parameters are reset to the default values
    _attachX.setValueAsPartOfGesture(_rapX.convertFrom0to1(_rapX.getDefaultValue()));
    _attachY.setValueAsPartOfGesture(_rapY.convertFrom0to1(_rapY.getDefaultValue()));
}

void DraggableComponent::setDragableComponentLabel(juce::String& newName)
{
    juce::Font labelFont;

    labelFont.setHorizontalScale(0.9f);
    labelFont.setHeight(15.5f);
    labelFont.setBold(true);
    _labelName.setJustificationType(juce::Justification::centred);
    _labelName.setFont(labelFont);
    _labelName.setText(newName, juce::dontSendNotification);
}

juce::Label* DraggableComponent::getDragableComponentLabel()
{
    return &_labelName;
}
void DraggableComponent::setDraggableComponentColour(juce::Colour newColour)
{
    _colour = newColour;
    
}

juce::Colour DraggableComponent::getDraggableComponentColour()
{
    return _colour;
}