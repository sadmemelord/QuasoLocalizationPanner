/*
  ==============================================================================

    ComponentsProperties.cpp
    Created: 2 May 2023 5:57:45pm
    Author:  Alberto

  ==============================================================================
*/

#include "../../PluginEditor.h"

void MultitrackPannerAudioProcessorEditor::attachComponents()
{
    //Buttons are attached to the APVTS' Active Track parameter
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        juce::StringArray activeIds = audioProcessor.getActiveTracksIDs();
        textButtonsAttach.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, activeIds[channel], *textButtons[channel]));
    }

    //The output volume slider is attached to the output parameter with the default Slider Attachment class
    masterVolumeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "output", masterVolumeSlider);
}

void MultitrackPannerAudioProcessorEditor::setTextButtonProperties()
{
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        //Each text buttons is linked to a draggable component, they share the same name, label and colour
        auto dragCompLabel = _dragComponents[channel]->getDragableComponentLabel();
        auto buttonText = dragCompLabel->getText();
        auto buttonName = _dragComponents[channel]->getName();
        auto buttonColour = _dragComponents[channel]->getDraggableComponentColour();

        addAndMakeVisible(textButtons[channel]);
        textButtons[channel]->setToggleable(true);
        textButtons[channel]->setButtonText(buttonText);
        textButtons[channel]->setName(buttonName);
        textButtons[channel]->setLookAndFeel(&buttonLookAndFeel);
        textButtons[channel]->setColour(juce::TextButton::ColourIds::buttonColourId, buttonColour);

        //The editor listens to the button to set the visibility of the draggable component
        //The editor listens to the draggable component's label changes to update the button's text
        textButtons[channel]->addListener(this);
        dragCompLabel->addListener(this);
    }
}

void MultitrackPannerAudioProcessorEditor::setSliderProperties()
{
    addAndMakeVisible(masterVolumeSlider);
    masterVolumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    masterVolumeSlider.setRange(-64.0f, +12.0f);
    masterVolumeSlider.setDoubleClickReturnValue(true, 0.0f);
    masterVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 35);
    masterVolumeSlider.setTextValueSuffix(" dB");
    masterVolumeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    masterVolumeSlider.setColour(juce::Slider::trackColourId, CustomColors::lightGrey);
    masterVolumeSlider.setColour(juce::Slider::backgroundColourId, CustomColors::darkerGrey.brighter(0.1f));
    masterVolumeSlider.setColour(juce::Slider::textBoxOutlineColourId, CustomColors::darkerGrey.withAlpha(0.0f));
    masterVolumeSlider.setLookAndFeel(&verticalSliderLookAndFeel);

    addAndMakeVisible(masterVolumeLabel);
    masterVolumeLabel.setText("Output", juce::dontSendNotification);
}

void  MultitrackPannerAudioProcessorEditor::setGroupProperties()
{
    addAndMakeVisible(buttonsGroup);
    buttonsGroup.setColour(buttonsGroup.outlineColourId, CustomColors::lightGrey.withAlpha(0.25f));
    buttonsGroup.setColour(buttonsGroup.textColourId, CustomColors::lightGrey);
    buttonsGroup.setText("Active Tracks");
}
void MultitrackPannerAudioProcessorEditor::setPanningWindowProperties()
{
    addAndMakeVisible(panningWindow);

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        //If the button is toggled off its related Draggable Component is not visible
        panningWindow.getChildComponent(channel)->setVisible(textButtons[channel]->getToggleState());

        //Restoring the draggable component's label text from the APVTS
       _dragComponents[channel]->setDragableComponentLabel(audioProcessor.loadComponentLabelText(*_dragComponents[channel]));

    }
}

void PanningWindow::setDraggableComponentProperties()
{
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        juce::String dragCompLabel = "Track ";
        dragCompLabel << channel + 1;
        auto dragCompColour = CustomColors::findCustomColourFromIndex(channel + 1);

        addAndMakeVisible(_draggableComponents[channel]);
        _draggableComponents[channel]->setDragableComponentLabel(dragCompLabel);
        _draggableComponents[channel]->setName(dragCompLabel);
        _draggableComponents[channel]->setDraggableComponentColour(dragCompColour);

        _draggableComponents[channel]->addComponentListener(this);
    }
}

void PanningWindow::setShadowProperties(juce::Graphics& g)
{
    //Each Draggable Component when dragged projects a shadow on the Panning Windows which is
    //removed when the component is released
    juce::Point<int> _shadowOffset(3, 3);
    auto _shadow = juce::DropShadow(juce::Colours::black, 10.0f, _shadowOffset);
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        if (_draggableComponents[channel]->isMouseButtonDown())
        {
            _shadow.radius = 10.0f;
            _shadow.drawForRectangle(g, _draggableComponents[channel]->getBounds());
        }
        else
        {
            _shadow.radius = 0.0f;
            _shadow.drawForRectangle(g, _draggableComponents[channel]->getBounds().reduced(50));
        }
        juce::DropShadower _shadower(_shadow);
        _shadower.setOwner(_draggableComponents[channel]);
    }
}