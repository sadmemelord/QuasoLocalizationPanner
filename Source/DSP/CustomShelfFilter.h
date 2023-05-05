/*
  ==============================================================================

    CustomShelfFilter.h
    Created: 27 Mar 2023 12:25:55pm
    Author:  Utente

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using Coefficients = juce::dsp::IIR::Filter<float>::CoefficientsPtr;


class CustomShelfFilter
{
public:

    void resetFilters();
    void prepareFilters(juce::dsp::ProcessSpec& spec);
    void processFilters(juce::dsp::AudioBlock<float>& block);
    void updateShelfFilters(std::vector<float>& newFilterGains);

private:

    int _inputChannels{ 16 };
    float _shelfFrequency = 2000.0f;
    float _shelfGain = 0.0f;
    float _shelfQuality = 1.5f;
    double _sampleRate = 44100.0f;

    //the vector has to be of unique_ptr because the Filter components aren't copyable but they can be moved
    //inside a vector using the move() method
    std::vector<std::unique_ptr<juce::dsp::IIR::Filter<float>>> _shelfFilters;

    void updateFilterCoefficients(Coefficients& old, Coefficients& replacements);


};