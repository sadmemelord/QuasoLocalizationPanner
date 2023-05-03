/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/PanningWindow.h"
#include "GUI/Data/CustomColors.h"
#include "GUI/LAF/ButtonLAF.h"
#include "GUI/LAF/VerticalSliderLAF.h"

//==============================================================================
/**
*/
class MultitrackPannerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    MultitrackPannerAudioProcessorEditor (MultitrackPannerAudioProcessor&);
    ~MultitrackPannerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;

    void setTextButtonProperties();
    void setSliderProperties();
    void setGroupProperties();
    void setPanningWindowProperties();
    void attachComponents();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MultitrackPannerAudioProcessor& audioProcessor;

    PanningWindow panningWindow;

    ButtonLAF buttonLookAndFeel;

    juce::GroupComponent buttonsGroup;

    juce::TextButton textButton1;
    juce::TextButton textButton2;
    juce::TextButton textButton3;
    juce::TextButton textButton4;
    std::vector<juce::TextButton*> textButtons{ &textButton1, &textButton2, &textButton3 , &textButton4 };
    std::vector< std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> textButtonsAttach;

    juce::Image logoImage;

    VerticalSliderLAF verticalSliderLookAndFeel;
    juce::Slider masterVolumeSlider;
    juce::Label masterVolumeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterVolumeAttach;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultitrackPannerAudioProcessorEditor)
};
