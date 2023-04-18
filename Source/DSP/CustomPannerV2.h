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

    void setPan(std::vector<float>& newPans, std::vector<bool>& newActiveTracks);

    void prepare(const juce::dsp::ProcessSpec& spec);

    void reset();

    void process(juce::dsp::AudioBlock<float>& block) noexcept
    {
        //each sample is panned by using its related left and right volume.
        //Then the whole is stereo output is written on the channel 0 and 1 (L/R)
       std::vector<juce::dsp::AudioBlock<float>> inputBlocks;

        for (int channel = 0; channel < _busNumber; ++channel)
        {
            inputBlocks.push_back(block.getSingleChannelBlock(channel));
        }

        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float sampleL = 0.0f;
            float sampleR = 0.0f;

            for (int channel = 0; channel < _busNumber; ++channel)
            {
                if (_activeTracks[channel] == false)
                {
                    inputBlocks[channel].setSample(0, sample, 0);
                    //_leftVolumes[bus].setTargetValue(0.0f);
                    //_rightVolumes[bus].setTargetValue(0.0f);
                }

                sampleL += inputBlocks[channel].getSample(0, sample) * _leftVolumes[channel].getNextValue();
                sampleR += inputBlocks[channel].getSample(0, sample) * _rightVolumes[channel].getNextValue();
            }

            //Channels 0 and 1 of the buffer are overwritten to be the main L and R output
            //to be able to make reverb sends of channel 0 and 1 they are copied onto 2 auxiliary channels
            //at the end of those in input
            block.setSample(_busNumber, sample, block.getSample(0, sample));
            block.setSample(_busNumber + 1, sample, block.getSample(1, sample));

            //the main stereo output uses channel 0 and 1 of the audio buffer
            block.setSample(0, sample, sampleL);
            block.setSample(1, sample, sampleR);
        }

    }

private:


    std::vector<float> _pans;
    std::vector<juce::SmoothedValue<float>> _leftVolumes;
    std::vector<juce::SmoothedValue<float>>_rightVolumes;
    std::vector<bool> _activeTracks;
    int _busNumber { 4 };
    double _sampleRate = 44100.0;

    void update();
};
