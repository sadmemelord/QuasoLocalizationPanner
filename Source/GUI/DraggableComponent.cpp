/*
  ==============================================================================

    DraggableComponent.cpp
    Created: 23 Feb 2023 5:22:45pm
    Author:  Utente

  ==============================================================================
*/

#include "DraggableComponent.h"

DraggableComponent::DraggableComponent()
{
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
}
void DraggableComponent::mouseUp(const juce::MouseEvent& event)
{
    _labelName.setVisible(true);
}
void DraggableComponent::mouseDrag(const juce::MouseEvent& event)
{
    _dragger.dragComponent(this, event, &_constrainer);
    _labelName.setVisible(false);

    //_attach.setValueAsPartOfGesture(_rap.convertFrom0to1(getX()));
}

void DraggableComponent::mouseDoubleClick(const juce::MouseEvent& event)
{
    centreWithSize(50, 50);
    setCentrePosition(getParentWidth() / 2, 3 * getParentHeight() / 4 - this->getHeight() / 4);
}
void DraggableComponent::mouseWheelMove(const juce::MouseEvent& event,
                                        const juce::MouseWheelDetails& wheel)
{
    auto& random = juce::Random::getSystemRandom();
    _colour.fromRGB(random.nextInt(256),
                    random.nextInt(256),
                    random.nextInt(256));

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