/*
  ==============================================================================

    DraggableComponent.cpp
    Created: 23 Feb 2023 5:22:45pm
    Author:  Utente

  ==============================================================================
*/

#include "DraggableComponent.h"

DraggableComponent::DraggableComponent(juce::RangedAudioParameter* rapX, juce::RangedAudioParameter* rapY) :
    _rapX(*rapX),
    _rapY(*rapY),
    _attachX(_rapX, [this](float) {repaint(); }),
    _attachY(_rapY, [this](float) {repaint(); }),
    _compX(juce::jmap(rapX->getValue(),-300.f,300.f)),
    _compY(juce::jmap(rapY->getValue(), 0.f, 200.f))
{
    _attachX.sendInitialUpdate();
    _attachY.sendInitialUpdate();

    addAndMakeVisible(_labelName);
    _labelName.setColour(juce::Label::textColourId, juce::Colours::black);
    _labelFont.setBold(true);
    _labelName.setFont(_labelFont);

    _constrainer.setMinimumOnscreenAmounts(50, 50, 50, 50);
}

DraggableComponent::~DraggableComponent()
{

}

//==============================================================================
void DraggableComponent::paint(juce::Graphics& g)
{
    g.setColour(_colour);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 5.0f);
}

void DraggableComponent::resized()
{
    float labelX = 2.5;
    float labelHeight = 25;
    _labelName.setBounds(labelX, getHeight() / 2 - labelHeight / 2, getWidth() - 2 * labelX, labelHeight);
}

void DraggableComponent::mouseDown(const juce::MouseEvent& event)
{
    _dragger.startDraggingComponent(this, event);
    _attachX.beginGesture();
    _attachY.beginGesture();
}
void DraggableComponent::mouseUp(const juce::MouseEvent& event)
{
    _labelName.setVisible(true);
    _attachX.endGesture();
    _attachY.endGesture();
}
void DraggableComponent::mouseDrag(const juce::MouseEvent& event)
{
    _dragger.dragComponent(this, event, &_constrainer);
    _labelName.setVisible(false);

    _attachX.setValueAsPartOfGesture((double)(_compX / 300.f));
    _attachY.setValueAsPartOfGesture((double)(_compY / 200.f));
}

void DraggableComponent::mouseDoubleClick(const juce::MouseEvent& event)
{
    centreWithSize(50, 50);
    setCentrePosition(getParentWidth() / 2, 3 * getParentHeight() / 4 - this->getHeight() / 4);
    _attachX.setValueAsPartOfGesture(_rapX.convertFrom0to1(_rapX.getDefaultValue()));
    _attachY.setValueAsPartOfGesture(_rapY.convertFrom0to1(_rapY.getDefaultValue()));
}
void DraggableComponent::mouseWheelMove(const juce::MouseEvent& event,
                                        const juce::MouseWheelDetails& wheel)
{

}

void DraggableComponent::setDragableComponentLabel(juce::String& newName)
{
    _labelName.setText(newName, juce::dontSendNotification);
}

void DraggableComponent::setDraggableComponentColour(juce::Colour newColour)
{
    _colour = newColour;
    
}

juce::Colour DraggableComponent::getDraggableComponentColour()
{
    return _colour;
}