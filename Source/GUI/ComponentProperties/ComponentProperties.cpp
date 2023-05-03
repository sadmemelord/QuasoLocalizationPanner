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
    //attaching buttons to the activeTracks parameter in the APVTS
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        juce::StringArray activeIds = audioProcessor.getActiveTracksIDs();
        textButtonsAttach.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, activeIds[channel], *textButtons[channel]));
    }

    //attaching master volume to the Output parameter in the APVTS
    masterVolumeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "output", masterVolumeSlider);

}

void MultitrackPannerAudioProcessorEditor::setTextButtonProperties()
{
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        addAndMakeVisible(textButtons[channel]);

        textButtons[channel]->setToggleable(true);

        juce::String textButtonName = "Track ";
        textButtonName << channel + 1;
        textButtons[channel]->setButtonText(textButtonName);
        textButtons[channel]->setName(textButtonName);
        textButtons[channel]->setLookAndFeel(&buttonLookAndFeel);

        auto newButtonColour = panningWindow.getChildColour(channel);

        textButtons[channel]->setColour(juce::TextButton::ColourIds::buttonColourId, newButtonColour);
        textButtons[channel]->addListener(this);
    }


}

void MultitrackPannerAudioProcessorEditor::setSliderProperties()
{
    addAndMakeVisible(masterVolumeSlider);
    masterVolumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    masterVolumeSlider.setRange(-64.0f, +12.0f, 0.1f);
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
        panningWindow.getChildComponent(channel)->setVisible(textButtons[channel]->getToggleState());
    }


}
void PanningWindow::setDraggableComponentProperties()
{
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        //addAndMakeVisible(_draggableComponents[channel]);
        addChildComponent(_draggableComponents[channel]);

        juce::String _dragCompLabel = "Track ";
        _dragCompLabel << channel + 1;
        _draggableComponents[channel]->setDragableComponentLabel(_dragCompLabel);
        _draggableComponents[channel]->setName(_dragCompLabel);

        auto& random = juce::Random::getSystemRandom();
        juce::Colour colour(random.nextInt(256),
            random.nextInt(256),
            random.nextInt(256));
        _draggableComponents[channel]->setDraggableComponentColour(colour);

        _draggableComponents[channel]->addComponentListener(this);

    }
}

void PanningWindow::setShadowProperties(juce::Graphics& g)
{
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