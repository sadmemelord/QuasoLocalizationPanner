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
    CustomGain()
    {
        for (int bus = 0; bus < _busNumber; ++bus)
        {
            _gains.push_back(0.0f);
        }
    }

    /** Applies a new gain as a decibel value. */
    void setGainsDecibels(std::vector<float>& newGainsDecibels) noexcept 
    { 
        for (int bus = 0; bus < _busNumber; ++bus)
        {
            _gains[bus].setTargetValue((juce::Decibels::decibelsToGain<float>(newGainsDecibels[bus])));
        }
    }

    /** Sets the length of the ramp used for smoothing gain changes. */
    void setRampDurationSeconds(double newDurationSeconds) noexcept
    {
        if (_rampDurationSeconds != newDurationSeconds)
        {
            _rampDurationSeconds = newDurationSeconds;
            reset();
        }
    }

    /** Called before processing starts. */
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept
    {
        sampleRate = spec.sampleRate;
        reset();
    }

    /** Resets the internal state of the gain */
    void reset() noexcept
    {
        for (int bus = 0; bus < _busNumber; ++bus) 
        {
           
            _gains[bus].reset(sampleRate, _rampDurationSeconds);
        }
    }

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
    double sampleRate = 0, _rampDurationSeconds = 0;
    int _busNumber{ 4 };

};