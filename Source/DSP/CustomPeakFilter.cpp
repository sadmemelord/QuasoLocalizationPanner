/*
  ==============================================================================

    CustomPeakFilter.cpp
    Created: 15 Feb 2023 4:27:49pm
    Author:  Utente

  ==============================================================================
*/

#include "CustomPeakFilter.h"

void CustomPeakFilter::resetFilters()
{
    //the vector of filters is filled with IIR filters up to the number of input channels defined by _busNumber
    for (int channel = 0; channel < _busNumber; ++channel)
    {
        auto filter = std::make_unique<juce::dsp::IIR::Filter<float>>();

        _peakFilters.push_back(std::move(filter));
    }
}
void CustomPeakFilter::prepareFilters(juce::dsp::ProcessSpec& spec)
{
    //each filter in the vector prepares the DSP spec
    _sampleRate = spec.sampleRate;

    for (int channel = 0; channel < _busNumber; ++channel)
    {
        _peakFilters[channel]->prepare(spec);
    }
}
void CustomPeakFilter::processFilters(juce::dsp::AudioBlock<float>& block)
{
    //each filter in the vector processes a single channel of the whole audio block passed
    for (int channel = 0; channel < _busNumber; ++channel)
    {
        auto block1 = block.getSingleChannelBlock(channel);
        juce::dsp::ProcessContextReplacing<float> context(block1);

       _peakFilters[channel]->process(context);
    }
}

void CustomPeakFilter::updatePeakFilters(std::vector<float>& newFilterGains)
{
    //the new gain values for each filter are updated from the APVTS
    for (int channel = 0; channel < _busNumber; ++channel)
    {
        auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(_sampleRate, _peakFrequency, 
                                                                                    _peakQuality, juce::Decibels::decibelsToGain(newFilterGains[channel]));

        updateFilterCoefficients(_peakFilters[channel]->coefficients, peakCoefficients);
    }
}


void CustomPeakFilter::updateFilterCoefficients(Coefficients& old, Coefficients& replacements)
{
    //coefficients are updated
    *old = *replacements;
}