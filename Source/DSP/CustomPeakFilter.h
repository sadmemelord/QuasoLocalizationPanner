/*
  ==============================================================================

    CustomPeakFilter.h
    Created: 15 Feb 2023 4:27:49pm
    Author:  Utente

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using PeakFilter = juce::dsp::IIR::Filter<float>;
using Coefficients = PeakFilter::CoefficientsPtr;

class CustomPeakFilter
{
public:

    void resetFilters();
    void prepareFilters(juce::dsp::ProcessSpec& spec);
    void processFilters(juce::dsp::AudioBlock<float>& block);
    void updatePeakFilters(std::vector<float>& newFilterGains);

private:
    int _inputChannels{ 4 };
    float _peakFrequency = 3500.0f;
    float _peakQuality = 2.0f;
    float _peakGain = 0.0f;
    double _sampleRate = 44100.0f;

    //the vector has to be of unique_ptr because the Filter components aren't copyable but they can be moved
    //inside a vector using the move() method
    std::vector<std::unique_ptr<juce::dsp::IIR::Filter<float>>> _peakFilters;

    void updateFilterCoefficients(Coefficients& old, Coefficients& replacements);

};