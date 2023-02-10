/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters/Parameters.h"
#include "DSP/CustomPanner.h"

//==============================================================================
/**
*/
class MultitrackPannerAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    MultitrackPannerAudioProcessor();
    ~MultitrackPannerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;

private:
    //two main methods are needed for the apvts a custom method to creater the parameter layout
   //and an overrided method to listen to parameters changed for the GUI
   //to do so the AudioProcessor class needs to inherit also from juce::AudioProcessorValueTreeState::Listener
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //every time a parameter in the GUI is changed this method calls updateParameters
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    //this method updates every parameter in the apvts
    void updateParameters();


    /*juce::dsp::Panner<float> panModule1;
    juce::dsp::Panner<float> panModule2;*/

    CustomPanner customPanModule1;
    CustomPanner customPanModule2;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultitrackPannerAudioProcessor)
};
