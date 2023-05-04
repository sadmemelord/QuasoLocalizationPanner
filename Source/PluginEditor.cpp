/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultitrackPannerAudioProcessorEditor::MultitrackPannerAudioProcessorEditor (MultitrackPannerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    _dragComp1(p.apvts.getParameter("pan1"), p.apvts.getParameter("distance1")),
    _dragComp2(p.apvts.getParameter("pan2"), p.apvts.getParameter("distance2")),
    _dragComp3(p.apvts.getParameter("pan3"), p.apvts.getParameter("distance3")),
    _dragComp4(p.apvts.getParameter("pan4"), p.apvts.getParameter("distance4")),
    panningWindow(&_dragComponents)

{
    attachComponents();

    setPanningWindowProperties();
    setTextButtonProperties();
    setSliderProperties();
    setGroupProperties();

    logoImage = juce::ImageCache::getFromMemory(BinaryData::QPP_logo_black_png, BinaryData::QPP_logo_black_pngSize);

    setSize(800, 500);

    juce::AudioProcessorEditor::setResizable(false, false);
}

MultitrackPannerAudioProcessorEditor::~MultitrackPannerAudioProcessorEditor()
{
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        textButtons[channel]->setLookAndFeel(nullptr);
    }
    textButtons.clear();
    textButtons.shrink_to_fit();
}

void MultitrackPannerAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    int buttonIndex;
    bool toggleState = !button->getToggleState();

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        if (textButtons[channel] == button)
            buttonIndex = channel;
    }

    button->setToggleState(toggleState, false);
    panningWindow.getChildComponent(buttonIndex)->setVisible(toggleState);
}
//==============================================================================
void MultitrackPannerAudioProcessorEditor::paint (juce::Graphics& g)
{
  
    g.setGradientFill(juce::ColourGradient::vertical(CustomColors::darkerGrey, getHeight(),
        CustomColors::darkerGrey.brighter(0.05f), getHeight() * 0.5f));
    g.fillRect(getLocalBounds());

    g.drawImageWithin(logoImage, 100, 10, 600, 60, juce::RectanglePlacement::fillDestination, false);
}


void MultitrackPannerAudioProcessorEditor::resized()
{
    float panningWindowX = 75;
    float panningWindowY = 75;
    float textButtonWidth = 70;
    float textButtonHeight = 35;
    float buttonDistance = 11.25;
    float buttonGroupX = 50;
    float buttonGroupY = 20;

    panningWindow.setBounds(panningWindowX, panningWindowY, getWidth() - 2 * panningWindowX, getHeight() / 2);

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        textButtons[channel]->setBounds(panningWindowX + channel * textButtonWidth + channel * buttonDistance,
            panningWindow.getBottom() + 50, textButtonWidth, textButtonHeight);
    }

    buttonsGroup.setBounds(buttonGroupX, panningWindow.getBottom() + buttonGroupY, panningWindow.getWidth() + buttonGroupX, getBottom() - panningWindow.getBottom() - 2 * buttonGroupY);

    masterVolumeSlider.setBounds(panningWindow.getRight(), 1.5 * panningWindowY, getRight() - panningWindow.getRight(), panningWindow.getBottom() - 1.5 * panningWindowY);
    masterVolumeLabel.setBounds(masterVolumeSlider.getX() + 10, masterVolumeSlider.getY() - 30, masterVolumeSlider.getWidth(), 30);
}

