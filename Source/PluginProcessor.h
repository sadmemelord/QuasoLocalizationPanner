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

    //IDs and Names for each parameter of the APVTS are stored in String Arrays
    juce::StringArray panIDs;
    juce::StringArray panNames;
    juce::StringArray gainIDs;
    juce::StringArray gainNames;
    juce::StringArray peakFilterGainIDs;
    juce::StringArray peakFilterGainNames;
    juce::StringArray shelfFilterGainIDs;
    juce::StringArray shelfFilterGainNames;
    juce::StringArray activeTracksIDs;
    juce::StringArray activeNames;

    //ID and Name are needed to search a specific parameter in the APVTS, these methods defined in the Parameters.cpp file
    //return a string array containing IDs and Names for each parameter
    juce::StringArray getPanIDs();
    juce::StringArray getPanNames();
    juce::StringArray getGainIDs();
    juce::StringArray getGainNames();
    juce::StringArray getPeakFilterGainIDs();
    juce::StringArray getPeakFilterGainNames();
    juce::StringArray getShelfFilterGainIDs();
    juce::StringArray getShelfFilterGainNames();
    juce::StringArray getActiveTracksIDs();
    juce::StringArray getActiveTracksNames();


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

    //The CustomPeakFilter class allows the individual filtering of an arbitrary number of mono tracks inside an AudioBlock.
    //this class provides a number of default juce::dsp Peak Filters up to the input channels (defined as _busNumber),
    //each filter has fixed Frequency and Quality factor values and a variable gain. New gain values for each filter are
    //passed by reference to the updatePeakFilters method as a vector of floats whose size has to match the _busNumber variable 
    //defined in the class private section. The centre frequency and Q values are set to match the audible feeling of an
    //audio source distance from the listener
    CustomPeakFilter customPeakFilterModule;

    //The CustomShelfFilter class allows the individual filtering of an arbitrary number of mono tracks inside an AudioBlock.
    //this class provides a number of default juce::dsp  High Shelf Filters up to the input channels (defined as _busNumber),
    //each filter has fixed Frequency and Quality factor values and a variable gain. New gain values for each filter are
    //passed by reference to the updateShelfFilters method as a vector of floats whose size has to match the _busNumber variable 
    //defined in the class private section. The centre frequency and Q values are set to match the audible feeling of an
    //audio source distance from the listener
    CustomShelfFilter customShelfFilterModule;

    //This vector stores the new pan values passed to the customPanModuleV2.setPan() method, its size has to be equal 
    //to the _busNumber private variable in the CustomPannerV2 class
    std::vector<float> newPans;
    std::vector<float> newGains;
    std::vector<float> newPeakFilterGains;
    std::vector<float> newShelfFilterGains;
    std::vector<bool>  activeTracks;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultitrackPannerAudioProcessor)
};
