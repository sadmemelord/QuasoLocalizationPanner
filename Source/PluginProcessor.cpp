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
                       .withInput("Input", juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       ),
                       apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{

    //getting every parameter ID with the related methods defined in the Parameters.cpp file
    activeTracksIDs = getActiveTracksIDs();
    distanceIDs = getDistanceIDs();
    panIDs = getPanIDs();

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        //adding every apvts parameter listener
        apvts.addParameterListener(activeTracksIDs[channel], this);
        apvts.addParameterListener(distanceIDs[channel], this);
        apvts.addParameterListener(panIDs[channel], this);
        apvts.addParameterListener("output", this);

        //initializing every input track as active
        activeTracks.push_back(true);

        //initializing vectors of parameters passed to the DSP modules
        newGains.push_back(0.0f);
        newPeakFilterGains.push_back(0.0f);
        newShelfFilterGains.push_back(0.0f);
        newPans.push_back(0.0f);
        newOutputGains.push_back(0.0f);
        newDistances.push_back(0.0f);
    }
}

MultitrackPannerAudioProcessor::~MultitrackPannerAudioProcessor()
{
    //removing every parameter listener
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        apvts.removeParameterListener(activeTracksIDs[channel], this);
        apvts.removeParameterListener(distanceIDs[channel], this);
        apvts.removeParameterListener(panIDs[channel], this);
        apvts.removeParameterListener("output", this);
    }

    //cleaaring arrays of strings
    activeTracksIDs.clear();
    distanceIDs.clear();
    panIDs.clear();

    //clearing vectors
    activeTracks.clear();
    newGains.clear();
    newPeakFilterGains.clear();
    newShelfFilterGains.clear();
    newPans.clear();
    newOutputGains.clear();
    newDistances.clear();
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
    juce::StringArray pActiveIDs = getActiveTracksIDs();
    juce::StringArray pActiveNames = getActiveTracksNames();
    juce::StringArray pDistanceIDs = getDistanceIDs();
    juce::StringArray pDistanceNames = getDistanceNames();
    juce::StringArray pPanIDs = getPanIDs();
    juce::StringArray pPanNames = getPanNames();

    //pushing the global Output gain inside the parameters vector
    params.push_back(std::make_unique<juce::AudioParameterFloat>("output", "Output", -64.0f, +12.0f, 0.0f));

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        //every input track has to be panned in the stereo field L/R and placed at a certain distance
        //Movement in the L/R stereo field is made with a panner.
        //The distance feel is made with a combination of Gain, Filtering and Reverb

        //Every APVTS parameter is pushed in a vector
        params.push_back(std::make_unique<juce::AudioParameterBool>(pActiveIDs[channel], pActiveNames[channel], true));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(pDistanceIDs[channel], pDistanceNames[channel], 0.0f, 1.0f, 0.75f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(pPanIDs[channel], pPanNames[channel], -1.0f, 1.0f, 0.0f));
    }

    return{ params.begin(), params.end() };

}

void MultitrackPannerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();
}


void MultitrackPannerAudioProcessor::updateParameters()
{
    //loading the APVTS parameters, load() methos is needed because the values are Atomic
    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        activeTracks[channel] = apvts.getRawParameterValue(activeTracksIDs[channel])->load();

        //Gain and Filter parameters are all mapped based on the Distance value using the jmap method
        newGains[channel] = juce::jmap(apvts.getRawParameterValue(distanceIDs[channel])->load(),-18.0f,6.0f);
        newPeakFilterGains[channel] = juce::jmap(apvts.getRawParameterValue(distanceIDs[channel])->load(),-4.5f, 1.5f);
        newShelfFilterGains[channel] = juce::jmap(apvts.getRawParameterValue(distanceIDs[channel])->load(), -3.0f, 1.0f);

        newPans[channel] = apvts.getRawParameterValue(panIDs[channel])->load();
        newDistances[channel] = apvts.getRawParameterValue(distanceIDs[channel])->load();

        newOutputGains[channel] = apvts.getRawParameterValue("output")->load();
    }

    //updating DSP modules parameter with those linked in the APVTS
    customGainModule.setGainsDecibels(newGains);
    customPeakFilterModule.updatePeakFilters(newPeakFilterGains);
    customShelfFilterModule.updateShelfFilters(newShelfFilterGains);
    customPanModuleV2.setPan(newPans, activeTracks);
    customReverbSendsModule.setReverbSendsGains(newDistances);
    customOutputGainModule.setGainsDecibels(newOutputGains);
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
    //Defining the DSP spec to easily pass useful data to the DSP modules
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    //Preparing DSP spec for the custom gain module and setting the ramp time between to values for smoothing
    customGainModule.prepare(spec);
    customGainModule.setRampDurationSeconds(0.02f);
    customOutputGainModule.prepare(spec);
    customOutputGainModule.setRampDurationSeconds(0.02f);

    //preparing DSP spec for the custom filter module
    //the resetFilters() method fills the vector of unique pointers to filters definied in the class
    customPeakFilterModule.resetFilters();
    customPeakFilterModule.prepareFilters(spec);
    customShelfFilterModule.resetFilters();
    customShelfFilterModule.prepareFilters(spec);

    //Preparing the DSP spec for the customPanModuleV2.
    //In the CustomPannerV2 class' methods sampleRate and numChannels aren't used
    customPanModuleV2.prepare(spec);

    customReverbSendsModule.prepare(spec);
    customReverbSendsModule.setRampDurationSeconds(0.02f);

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

    //The default buffer size is set to 2 where the channel 0 and 1 represent the left and right channels.
    //With this method the buffer channel size can be modified to accomodate a larger number of channels.
    //Every channel in the buffer contains incoming input data while the stereo output data is written on channel 0 (L) and 1 (R).
    //the Buffer size has two more channel than those those in input to internally route the channel 0 and 1 to the channel N and N+1
    //for the reverb sends
    buffer.setSize(INPUTCHANNELS + 2, buffer.getNumSamples(), false, false, false);

    //The AudioBlock contains pointer to every channel that contains data, in this case it contains a pointer to every channel
    //of the AudioBuffer, this prevents the processing methods of the DSP classes from modifying the buffer directly
    juce::dsp::AudioBlock<float> block(buffer);

    customGainModule.process(block);
    customPeakFilterModule.processFilters(block);
    customShelfFilterModule.processFilters(block);
    customOutputGainModule.process(block);
    customPanModuleV2.process(block);
    customReverbSendsModule.process(block);



}

//==============================================================================
bool MultitrackPannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultitrackPannerAudioProcessor::createEditor()
{
    return new MultitrackPannerAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
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

void MultitrackPannerAudioProcessor::saveComponentLabelText(DraggableComponent& dragComp)
{
    auto labelText = dragComp.getDragableComponentLabel()->getText();
    auto label = apvts.state.getOrCreateChildWithName(dragComp.getName(), nullptr);
    label.setProperty(dragComp.getName(), labelText, nullptr);
}

juce::String MultitrackPannerAudioProcessor::loadComponentLabelText(DraggableComponent& dragComp)
{

    auto label = apvts.state.getOrCreateChildWithName(dragComp.getName(), nullptr);
    return label.getProperty(dragComp.getName(), dragComp.getName());
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultitrackPannerAudioProcessor();
}
