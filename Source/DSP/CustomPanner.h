/*
  ==============================================================================

    CustomPanner.h
    Created: 10 Feb 2023 6:44:17pm
    Author:  Utente

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomPanner
{
public:
    CustomPanner();

    void setPan(float newPan1, float newPan2);
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::dsp::AudioBlock<float>& block) noexcept
    {
        auto inputBlock1 = block.getSingleChannelBlock(0);
        auto inputBlock2 = block.getSingleChannelBlock(1);

    
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            auto currentSample1 = inputBlock1.getSample(0, sample);
            auto currentSample2 = inputBlock2.getSample(0, sample);

            block.setSample(0, sample, currentSample1 * _leftVolume1 + currentSample2 * _leftVolume2);
            block.setSample(1, sample, currentSample1 * _rightVolume1 + currentSample2 * _rightVolume2);

        }
        
        



    }

private:
    float _pan1 = 0.0f;
    float _pan2 = 0.0f;

    float _leftVolume1;
    float _rightVolume1;

    float _leftVolume2;
    float _rightVolume2;

    double _sampleRate = 44100.0;

    void update();
};
