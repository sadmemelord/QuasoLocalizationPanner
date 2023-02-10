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
    apvts.addParameterListener(pan1ID, this);
    apvts.addParameterListener(pan2ID, this);
}

MultitrackPannerAudioProcessor::~MultitrackPannerAudioProcessor()
{
    apvts.removeParameterListener(pan1ID, this);
    apvts.removeParameterListener(pan2ID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout MultitrackPannerAudioProcessor::createParameterLayout()
{
    //parameters of the apvts are stored in a vector as unique_pointers to RangedAudioParamter
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    auto pPan1 = std::make_unique<juce::AudioParameterFloat>(pan1ID, pan1Name, -1.0f, 1.0f, 0.0f);
    auto pPan2 = std::make_unique<juce::AudioParameterFloat>(pan2ID, pan2Name, -1.0f, 1.0f, 0.0f);

    params.push_back(std::move(pPan1));
    params.push_back(std::move(pPan2));

    return{ params.begin(), params.end() };

}

void MultitrackPannerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();
}


void MultitrackPannerAudioProcessor::updateParameters()
{
    //panModule1.setPan(apvts.getRawParameterValue("pan1")->load());
    //panModule2.setPan(apvts.getRawParameterValue("pan2")->load());
    customPanModule1.setPan(apvts.getRawParameterValue(pan1ID)->load(), apvts.getRawParameterValue(pan2ID)->load());
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

   /* panModule1.prepare(spec);
    panModule1.setRule(juce::dsp::PannerRule::balanced);
    panModule2.prepare(spec);
    panModule2.setRule(juce::dsp::PannerRule::balanced);*/

    customPanModule1.prepare(spec);

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
    //auto block1 = block.getSingleChannelBlock(0);
    //juce::dsp::ProcessContextNonReplacing<float> context1(block1,block);
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
