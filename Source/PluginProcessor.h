/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DSP/CustomPannerV2.h"
#include "DSP/CustomGain.h"
#include "DSP/CustomPeakFilter.h"
#include "DSP/CustomShelfFilter.h"
#include "DSP/CustomReverbSends.h"
#include "GUI/DraggableComponent.h"

//Memory for each vector of parameters is allocated in the AudioProcessor constructor,
//to do so the vector size (which represents the input channels of the VST) is defined
//as a preprocessor statement
#define INPUTCHANNELS 16

//==============================================================================
/**
*/
class QuasoLocalizationPannerAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    QuasoLocalizationPannerAudioProcessor();
    ~QuasoLocalizationPannerAudioProcessor() override;

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
    void saveComponentLabelText(DraggableComponent& dragComp);
    juce::String loadComponentLabelText(DraggableComponent& dragComp);


    //==============================================================================

   //To work properly the APVTS needs the following:
   //-A method to create the ParameterLayout, called in the AudioProcessor class constructor, here every parameter that
   //will display in the GUI is createrd and pushed in a vector.
   //-An overrided method to listen to parameters changes in the GUI, and to do so the AudioProcessor class needs to 
   //inherit also from juce::AudioProcessorValueTreeState::Listener
    juce::AudioProcessorValueTreeState apvts;

    //ID and Name are needed to search a specific parameter in the APVTS, these methods defined in the Parameters.cpp file
    //return a string array containing IDs and Names for each parameter visible in the GUI
    juce::StringArray getActiveTracksIDs();
    juce::StringArray getActiveTracksNames();
    juce::StringArray getDistanceIDs();
    juce::StringArray getDistanceNames();
    juce::StringArray getPanIDs();
    juce::StringArray getPanNames();

private:

    //In this method called in the AudioProcessor class constructor, every parameter that will be displayed
    //in the GUI is createrd and pushed inside a parameters vector
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //This method is called everytime a parameter in the APVTS is changed by the user in the GUI
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    //This method updates the DSP modules parameters linked with those in the APVTS
    void updateParameters();

    //IDs and Names for each parameter of the APVTS are stored in String Arrays
    juce::StringArray activeTracksIDs;
    juce::StringArray distanceIDs;
    juce::StringArray panIDs;

    //The CustomPannerV2 class allows the individual panning of an arbitrary number of mono tracks on a stereo output.
    //The only panning method avaiable is "Balanced" panning rule copied from the default juce::dsp::Panner class).
    //New pan values for each track are passed by reference to the setPan method as a vector of floats, whose size
    //has to match the _busNumber variable defined in the class private section.
    CustomPannerV2 customPanModuleV2;

    //The CustomGain class allows to change the gain of an arbitrary number of input mono tracks inside an AudioBlock.
    //This class is based on the default DSP Gain class and implements the linear smoothing between gain values.
    //New gain values for each track are passed by reference to the setGain method as a vector of floats, whose size
    //has to match the _busNumber variable defined in the class private section.
    CustomGain customGainModule;

    //This module allows to change the global output gain of each channel with a single slider in the GUI
    CustomGain customOutputGainModule;

    //The CustomPeakFilter class allows the individual filtering of an arbitrary number of mono tracks inside an AudioBlock.
    //this class provides a number of default juce::dsp Peak Filters up to the input channels (defined as _channelNumber),
    //each filter has fixed Frequency and Quality factor values and a variable gain. New gain values for each filter are
    //passed by reference to the updatePeakFilters method as a vector of floats whose size has to match the _busNumber variable 
    //defined in the class private section. The centre frequency and Q values are set to match the audible feeling of an
    //audio source distance from the listener
    CustomPeakFilter customPeakFilterModule;

    //The CustomShelfFilter class allows the individual filtering of an arbitrary number of mono tracks inside an AudioBlock.
    //this class provides a number of default juce::dsp  High Shelf Filters up to the input channels (defined as _channelNumber),
    //each filter has fixed Frequency and Quality factor values and a variable gain. New gain values for each filter are
    //passed by reference to the updateShelfFilters method as a vector of floats whose size has to match the _busNumber variable 
    //defined in the class private section. The centre frequency and Q values are set to match the audible feeling of an
    //audio source distance from the listener
    CustomShelfFilter customShelfFilterModule;

    //reverb sends module
    CustomReverbSends customReverbSendsModule;

    //This vector stores the new dsp modules values, its size has to be equal 
    //to the _channelNumber private variable in the Custom classes
    std::vector<bool>  activeTracks;
    std::vector<float> newGains;
    std::vector<float> newPeakFilterGains;
    std::vector<float> newShelfFilterGains;
    std::vector<float> newPans;
    std::vector<float> newDistances;
    std::vector<float> newOutputGains;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuasoLocalizationPannerAudioProcessor)
};
