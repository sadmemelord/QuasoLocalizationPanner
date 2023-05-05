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
    _dragComp5(p.apvts.getParameter("pan5"), p.apvts.getParameter("distance5")),
    _dragComp6(p.apvts.getParameter("pan6"), p.apvts.getParameter("distance6")),
    _dragComp7(p.apvts.getParameter("pan7"), p.apvts.getParameter("distance7")),
    _dragComp8(p.apvts.getParameter("pan8"), p.apvts.getParameter("distance8")),
    _dragComp9(p.apvts.getParameter("pan9"), p.apvts.getParameter("distance9")),
    _dragComp10(p.apvts.getParameter("pan10"), p.apvts.getParameter("distance10")),
    _dragComp11(p.apvts.getParameter("pan11"), p.apvts.getParameter("distance11")),
    _dragComp12(p.apvts.getParameter("pan12"), p.apvts.getParameter("distance12")),
    _dragComp13(p.apvts.getParameter("pan13"), p.apvts.getParameter("distance13")),
    _dragComp14(p.apvts.getParameter("pan14"), p.apvts.getParameter("distance14")),
    _dragComp15(p.apvts.getParameter("pan15"), p.apvts.getParameter("distance15")),
    _dragComp16(p.apvts.getParameter("pan16"), p.apvts.getParameter("distance16")),
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
    float textButtonWidth = 75;
    float textButtonHeight = 35;
    float buttonDistance = 6;
    float buttonGroupX = 50;
    float buttonGroupY = 20;

    panningWindow.setBounds(panningWindowX, panningWindowY, getWidth() - 2 * panningWindowX, getHeight() / 2);

    buttonsGroup.setBounds(buttonGroupX, panningWindow.getBottom() + buttonGroupY, panningWindow.getWidth() + buttonGroupX, getBottom() - panningWindow.getBottom() - 2 * buttonGroupY);

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        if(channel <= 7)
       textButtons[channel]->setBounds(panningWindowX + 4 + channel * textButtonWidth + channel * buttonDistance,
                                       buttonsGroup.getY() + 30, textButtonWidth, textButtonHeight);
        else
            textButtons[channel]->setBounds(panningWindowX + 4 + (channel - 8) * textButtonWidth + (channel - 8) * buttonDistance,
                                            buttonsGroup.getBottom() - 25 - textButtonHeight, textButtonWidth, textButtonHeight);
    }



    masterVolumeSlider.setBounds(panningWindow.getRight(), 1.5 * panningWindowY, getRight() - panningWindow.getRight(), panningWindow.getBottom() - 1.5 * panningWindowY);
    masterVolumeLabel.setBounds(masterVolumeSlider.getX() + 10, masterVolumeSlider.getY() - 30, masterVolumeSlider.getWidth(), 30);
}

