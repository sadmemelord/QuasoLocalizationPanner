/*
  ==============================================================================

    CustomGain.h
    Created: 14 Feb 2023 5:36:11pm
    Author:  Utente

  ==============================================================================
*/

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


        for (int bus = 0; bus < _busNumber; ++bus)
        {
            inputBlocks.push_back(block.getSingleChannelBlock(bus));
        }

        for (int bus = 0; bus < _busNumber; ++bus)
        {
            for (int sample = 0; sample < block.getNumSamples(); ++sample)
            {
                block.setSample(bus, sample, inputBlocks[bus].getSample(0, sample) * _gains[bus].getNextValue());
            }
        }
    }


private:
   std::vector<juce::SmoothedValue<float>> _gains;
   double _sampleRate = 44100.0;
   double _rampDurationSeconds = 0.02;
   int _busNumber{ 4 };

};