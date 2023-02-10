/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MultitrackPannerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MultitrackPannerAudioProcessorEditor (MultitrackPannerAudioProcessor&);
    ~MultitrackPannerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MultitrackPannerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultitrackPannerAudioProcessorEditor)
};
