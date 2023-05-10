
#pragma once
#include <JuceHeader.h>

class CustomGain
{
public:
    CustomGain();

    void setGainsDecibels(std::vector<float>& newGainsDecibels) noexcept;

    void setRampDurationSeconds(double newDurationSeconds) noexcept;

    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;

    void reset() noexcept;

    void process(juce::dsp::AudioBlock<float>& block) noexcept
    {
        std::vector<juce::dsp::AudioBlock<float>> inputBlocks;


        for (int channel = 0; channel < _inputChannels; ++channel)
        {
            inputBlocks.push_back(block.getSingleChannelBlock(channel));
        }

        for (int channel = 0; channel < _inputChannels; ++channel)
        {
            for (int sample = 0; sample < block.getNumSamples(); ++sample)
            {
                block.setSample(channel, sample, inputBlocks[channel].getSample(0, sample) * _gains[channel].getNextValue());
            }
        }
    }


private:
   std::vector<juce::SmoothedValue<float>> _gains;
   double _sampleRate = 44100.0;
   double _rampDurationSeconds = 0.02;
   int _inputChannels{ 16 };

};