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
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                       apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    apvts.addParameterListener(gain1ID, this);
    apvts.addParameterListener(pan1ID, this);
    apvts.addParameterListener(gain2ID, this);
    apvts.addParameterListener(pan2ID, this);
}

MultitrackPannerAudioProcessor::~MultitrackPannerAudioProcessor()
{
    apvts.removeParameterListener(gain1ID, this);
    apvts.removeParameterListener(pan1ID, this);
    apvts.removeParameterListener(gain2ID, this);
    apvts.removeParameterListener(pan2ID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout MultitrackPannerAudioProcessor::createParameterLayout()
{
    //parameters of the apvts are stored in a vector as unique_pointers to RangedAudioParamter
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    auto pGain1 = std::make_unique<juce::AudioParameterFloat>(gain1ID, gain1Name, -12.0f, 12.0f, 0.0f);
    auto pPan1 = std::make_unique<juce::AudioParameterFloat>(pan1ID, pan1Name, -1.0f, 1.0f, 0.0f);

    auto pGain2 = std::make_unique<juce::AudioParameterFloat>(gain2ID, gain2Name, -12.0f, 12.0f, 0.0f);
    auto pPan2 = std::make_unique<juce::AudioParameterFloat>(pan2ID, pan2Name, -1.0f, 1.0f, 0.0f);

    params.push_back(std::move(pGain1));
    params.push_back(std::move(pPan1));
    params.push_back(std::move(pGain2));
    params.push_back(std::move(pPan2));

    return{ params.begin(), params.end() };

}

void MultitrackPannerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();
}


void MultitrackPannerAudioProcessor::updateParameters()
{
    customPanModule1.setPan(apvts.getRawParameterValue(pan1ID)->load(), apvts.getRawParameterValue(pan2ID)->load());
    gainModule1.setGainDecibels(apvts.getRawParameterValue(gain1ID)->load());
    gainModule2.setGainDecibels(apvts.getRawParameterValue(gain2ID)->load());
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
void MultitrackPannerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;

    customPanModule1.prepare(spec);

    gainModule1.prepare(spec);
    gainModule1.setRampDurationSeconds(0.02f);

    gainModule2.prepare(spec);
    gainModule2.setRampDurationSeconds(0.02f);


    enableAllBuses();
    updateParameters();
}

void MultitrackPannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool MultitrackPannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return true;
}


void MultitrackPannerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context1(block.getSingleChannelBlock(0));
    juce::dsp::ProcessContextReplacing<float> context2(block.getSingleChannelBlock(1));

    gainModule1.process(context1);
    gainModule2.process(context2);

    customPanModule1.process(block);

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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MultitrackPannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultitrackPannerAudioProcessor();
}
