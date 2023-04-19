/*
  ==============================================================================

    CustomReverbSends.h
    Created: 17 Apr 2023 4:58:48pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomReverbSends 
{
public:

    CustomReverbSends();
    void setRampDurationSeconds(double newDurationSeconds) noexcept;     
    void reset() noexcept;
    void setReverbSendsGains(std::vector<float>& distance);
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;

    void process(juce::dsp::AudioBlock<float>& block) noexcept
    {
        std::vector<juce::dsp::AudioBlock<float>> inputBlocks;

        for (int channel = 0; channel < _inputChannels + 2; ++channel)
        {
            //this for is going up until _inputChannels + 2 to include the 2 additional
            //channel to map the output 0 and 1
            inputBlocks.push_back(block.getSingleChannelBlock(channel));
        }

        for (int channel = 0; channel < _inputChannels; ++channel)
        {
            for (int sample = 0; sample < block.getNumSamples(); ++sample)
            {
                //the reverb send level for each channel is proportional to the distance from the listener
                if (channel == 0)
                {
                    //The reverb send of channel 0 is mapped onto channel N
                    block.setSample(_inputChannels, sample, inputBlocks[_inputChannels].getSample(0, sample) * _reverbSendsGains[channel].getNextValue());
                }
                else if (channel == 1)
                {
                    //the reverb send of channel 1 is mapped onto channel N+1
                    block.setSample(_inputChannels + 1, sample, inputBlocks[_inputChannels +1].getSample(0, sample) * _reverbSendsGains[channel].getNextValue());
                }
                else
                {
                    block.setSample(channel, sample, inputBlocks[channel].getSample(0, sample) * _reverbSendsGains[channel].getNextValue());
                }
            }
        }
    }

private:

      std::vector<juce::SmoothedValue<float>> _reverbSendsGains;
      double _sampleRate = 44100.0;
      double _rampDurationSeconds = 0.02;
      int _inputChannels{ 4 };
};