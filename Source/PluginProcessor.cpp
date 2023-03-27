/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultitrackPannerAudioProcessor::MultitrackPannerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput("Input_1", juce::AudioChannelSet::mono(), true)      
                       .withInput("Input_2", juce::AudioChannelSet::mono(), true)
                       .withInput("Input_3", juce::AudioChannelSet::mono(), true)
                       .withInput("Input_4", juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                       apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    //getting every parameter ID with the related methods defined in the Parameters.cpp file
    panIDs = getPanIDs();
    gainIDs = getGainIDs();
    activeTracksIDs = getActiveTracksIDs();
    peakFilterGainIDs = getPeakFilterGainIDs();
    shelfFilterGainIDs = getShelfFilterGainIDs();



    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
    {

        //adding every parameter listener
        apvts.addParameterListener(panIDs[bus], this);
        apvts.addParameterListener(gainIDs[bus], this);
        apvts.addParameterListener(peakFilterGainIDs[bus], this);
        apvts.addParameterListener(shelfFilterGainIDs[bus], this);
        apvts.addParameterListener(activeTracksIDs[bus], this);


        //initializing vectors of parameters passed to the DSP modules
        newPans.push_back(0.0f);
        newGains.push_back(0.0f);
        newPeakFilterGains.push_back(0.0f);
        newShelfFilterGains.push_back(0.0f);
        
        //initializing every input track as active
        activeTracks.push_back(true);
    }

}

MultitrackPannerAudioProcessor::~MultitrackPannerAudioProcessor()
{
    //removing every parameter listener
    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
    {
    apvts.removeParameterListener(panIDs[bus], this);
        apvts.removeParameterListener(gainIDs[bus], this);
        apvts.removeParameterListener(peakFilterGainIDs[bus], this);
        apvts.removeParameterListener(shelfFilterGainIDs[bus], this);
        apvts.removeParameterListener(activeTracksIDs[bus], this);

    }

    //cleaaring arrays of strings
    panIDs.clear();
    gainIDs.clear();
    peakFilterGainIDs.clear();
    shelfFilterGainIDs.clear();
    activeTracksIDs.clear();


    //clearing vectors
    newPans.clear();
    newGains.clear();
    newPeakFilterGains.clear();
    newShelfFilterGains.clear();
    activeTracks.clear();
}

juce::AudioProcessorValueTreeState::ParameterLayout MultitrackPannerAudioProcessor::createParameterLayout()
{
    //Additional buses are disabled by default this method enables them all
    //As of right now Juce is not working correctly with dynamic buses
    enableAllBuses();

    //parameters of the apvts are stored in a vector as unique_pointers to RangedAudioParamter
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //because the APVTS constructor is called before the AudioProcessor constructor
    //two local arrays for parameter names and IDs are created
    juce::StringArray pPanIDs = getPanIDs();
    juce::StringArray pPanNames = getPanNames();
    juce::StringArray pGainIDs = getGainIDs();
    juce::StringArray pGainNames = getGainNames();
    juce::StringArray pActiveIDs = getActiveTracksIDs();
    juce::StringArray pActiveNames = getActiveTracksNames();
    juce::StringArray pPeakFilterGainIDs = getPeakFilterGainIDs();
    juce::StringArray pPeakFilterGainNames = getPeakFilterGainNames();
    juce::StringArray pShelfFilterGainIDs = getShelfFilterGainIDs();
    juce::StringArray pShelfFilterGainNames = getShelfFilterGainNames();


    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
    {
        //every input track has to be panned in the stereo field L/R and placed at a certain distance
        //Movement in the L/R stereo field is made with a panner.
        //The distance feel is made with a combination of Gain, Filtering and Reverb
        //FUTURE UPDATE: PEAK FILTER AND MUTE BUTTON FOR EACH TRACK

        //Every APVTS parameter is pushed in a vector
        params.push_back(std::make_unique<juce::AudioParameterBool>(pActiveIDs[bus], pActiveNames[bus], true));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(pGainIDs[bus], pGainNames[bus], -6.0f, 6.0f, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(pPeakFilterGainIDs[bus], pPeakFilterGainNames[bus], -6.0f, 6.0f, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(pShelfFilterGainIDs[bus], pShelfFilterGainNames[bus], -6.0f, 6.0f, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(pPanIDs[bus], pPanNames[bus], -1.0f, 1.0f, 0.0f));


    }

    return{ params.begin(), params.end() };

}

void MultitrackPannerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();
}


void MultitrackPannerAudioProcessor::updateParameters()
{
    //loading the APVTS parameters, load() methos is needed because they're Atomic
    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
    {
        activeTracks[bus] = apvts.getRawParameterValue(activeTracksIDs[bus])->load();
        newGains[bus] = apvts.getRawParameterValue(gainIDs[bus])->load();
        newPeakFilterGains[bus] = apvts.getRawParameterValue(peakFilterGainIDs[bus])->load();
        newShelfFilterGains[bus] = apvts.getRawParameterValue(shelfFilterGainIDs[bus])->load();
        newPans[bus] = apvts.getRawParameterValue(panIDs[bus])->load();

    }

    //updating DSP modules parameter with those linked in the APVTS
    customGainModule.setGainsDecibels(newGains);
    customPeakFilterModule.updatePeakFilters(newPeakFilterGains);
    customShelfFilterModule.updateShelfFilters(newShelfFilterGains);
    customPanModuleV2.setPan(newPans, activeTracks);


}

//==============================================================================
const juce::String MultitrackPannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MultitrackPannerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MultitrackPannerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MultitrackPannerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MultitrackPannerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MultitrackPannerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MultitrackPannerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MultitrackPannerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MultitrackPannerAudioProcessor::getProgramName (int index)
{
    return {};
}

void MultitrackPannerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MultitrackPannerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    enableAllBuses();

    //Defining the DSP spec to easily pass useful data to the DSP modules
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    //Preparing DSP spec for the custom gain module and setting the ramp time between to values for smoothing
    customGainModule.prepare(spec);
    customGainModule.setRampDurationSeconds(0.02f);

    //preparing DSP spec for the custom filter module
    //the resetFilters() method fills the vector of unique pointers to filters definied in the class
    customPeakFilterModule.resetFilters();
    customPeakFilterModule.prepareFilters(spec);

    customShelfFilterModule.resetFilters();
    customShelfFilterModule.prepareFilters(spec);

    //Preparing the DSP spec for the customPanModuleV2.
    //In the CustomPannerV2 class' methods sampleRate and numChannels aren't used
    customPanModuleV2.prepare(spec);

    updateParameters();
}

void MultitrackPannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool MultitrackPannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    //if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo() || layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono())
    //    return false;
    //else
    //    return true;
    return true;
}


void MultitrackPannerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //The default buffer size is set to 2 where the channel 0 and 1 represent the left and right channels.
    //With this method the buffer channel size can be modified to accomodate a larger number of channels.
    //Every channel in the buffer contains incoming input data while the stereo output data is written on channel 0 (L) and 1 (R)
    buffer.setSize(totalNumInputChannels, buffer.getNumSamples(), false, false, false);

    //The AudioBlock contains pointer to every channel that contains data, in this case it contains a pointer to every channel
    //of the AudioBuffer, this prevents the processing methods of the DSP classes from modifying the buffer directly
    juce::dsp::AudioBlock<float> block(buffer);

    customGainModule.process(block);
    customPeakFilterModule.processFilters(block);
    customShelfFilterModule.processFilters(block);
    customPanModuleV2.process(block);


}

//==============================================================================
bool MultitrackPannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultitrackPannerAudioProcessor::createEditor()
{
    //return new MultitrackPannerAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void MultitrackPannerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //This methods saves the APVTS current parameters when the VST is closed
    juce::MemoryOutputStream    stream(destData, false);
    apvts.state.writeToStream(stream);
}

void MultitrackPannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //This methods restores the last saved APVTS parameters
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));

    if (tree.isValid())
    {
        apvts.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultitrackPannerAudioProcessor();
}
