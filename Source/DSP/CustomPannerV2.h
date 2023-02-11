/*
  ==============================================================================

    CustomPannerV2.h
    Created: 11 Feb 2023 3:08:12pm
    Author:  Utente

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/**
The CustomPannerV2 class allows the individual panning of an arbitrary number of mono tracks on a stereo output.
The only panning method avaiable is "Balanced" panning rule copied from the default juce::dsp::Panner class).
New pan values for each track are passed by reference to the setPan() method as a vector of floats, whose size 
has to match the _busNumber variable defined in the class private section.
*/

class CustomPannerV2
{
public:
    CustomPannerV2();

    void setPan(std::vector<float>& newPans);

    void prepare(const juce::dsp::ProcessSpec& spec);

    void reset();

    void process(juce::dsp::AudioBlock<float>& block) noexcept
    {
       std::vector<juce::dsp::AudioBlock<float>> inputBlocks;

        for (int bus = 0; bus < _busNumber; ++bus)
        {
            inputBlocks.push_back(block.getSingleChannelBlock(bus));
        }

        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float sampleL = 0.0f;
            float sampleR = 0.0f;

            for (int bus = 0; bus < _busNumber; ++bus)
            {
                sampleL += inputBlocks[bus].getSample(0, sample) * _leftVolumes[bus];
                sampleR += inputBlocks[bus].getSample(0, sample) * _rightVolumes[bus];
            }

            block.setSample(0, sample, sampleL);
            block.setSample(1, sample, sampleR);
        }

    }

private:


    std::vector<float> _pans;
    std::vector<float> _leftVolumes;
    std::vector<float> _rightVolumes;
    int _busNumber { 4 };
    double _sampleRate = 44100.0;

    void update();
};
