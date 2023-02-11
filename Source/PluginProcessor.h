/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters/Parameters.h"
#include "DSP/CustomPanner.h"
#include "DSP/CustomPannerV2.h"

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

   //To work properly the APVTS needs the following:
   //-A method to create the ParameterLayout, called in the AudioProcessor class constructor, here every parameter that
   //will display in the GUI is createrd and pushed in a vector.
   //-An overrided method to listen to parameters changes in the GUI, and to do so the AudioProcessor class needs to 
   //inherit also from juce::AudioProcessorValueTreeState::Listener
    juce::AudioProcessorValueTreeState apvts;

private:
    //In this method called in the AudioProcessor class constructor, every parameter that will be displayed
    //in the GUI is createrd and pushed inside a parameters vector
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //This method is called everytime a parameter in the APVTS is changed by the user in the GUI
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    //This method updates the DSP modules parameters linked with those in the APVTS
    void updateParameters();
    
    //Default DSP Gain modules, one for each track 
    //FUTURE UPDATE: CUSTOM DSP GAIN MODULES FOR MULTIPLE TRACKS
    juce::dsp::Gain<float> gainModule1;
    juce::dsp::Gain<float> gainModule2;
    juce::dsp::Gain<float> gainModule3;
    juce::dsp::Gain<float> gainModule4;


    //The CustomPannerV2 class allows the individual panning of an arbitrary number of mono tracks on a stereo output.
    //The only panning method avaiable is "Balanced" panning rule copied from the default juce::dsp::Panner class).
    //New pan values for each track are passed by reference to the setPan() method as a vector of floats, whose size
    //has to match the _busNumber variable defined in the class private section.
    //FUTURE UPDATE: BYPASS BUTTON FOR EACH TRACK
    CustomPannerV2 customPanModuleV2;

    //This vector stores the new pan values passed to the customPanModuleV2.setPan() method, its size has to be equal 
    //to the _busNumber private variable in the CustomPannerV2 class
    //FUTURE UPDATE: CHECK IF THE VECTOR CAN BE INITIALIZED IN THE CONSTRUCTOR WITH A CUSTOM METHOD
    std::vector<float> newPans = { 0.0f, 0.0f, 0.0f, 0.0f};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultitrackPannerAudioProcessor)
};
