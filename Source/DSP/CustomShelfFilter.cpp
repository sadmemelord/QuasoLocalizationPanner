/*
  ==============================================================================

    CustomShelfFilter.cpp
    Created: 27 Mar 2023 12:25:55pm
    Author:  Utente

  ==============================================================================
*/

#include "CustomShelfFilter.h"


void CustomShelfFilter::resetFilters()
{
    //the vector of filters is filled with IIR filters up to the number of input channels defined by _inputChannels
    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        auto filter = std::make_unique<juce::dsp::IIR::Filter<float>>();

        _shelfFilters.push_back(std::move(filter));
    }
}
void CustomShelfFilter::prepareFilters(juce::dsp::ProcessSpec& spec)
{
    //each filter in the vector prepares the DSP spec
    _sampleRate = spec.sampleRate;

    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        _shelfFilters[channel]->prepare(spec);
    }
}
void CustomShelfFilter::processFilters(juce::dsp::AudioBlock<float>& block)
{
    //each filter in the vector processes a single channel of the whole audio block passed
    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        auto block1 = block.getSingleChannelBlock(channel);
        juce::dsp::ProcessContextReplacing<float> context(block1);

        _shelfFilters[channel]->process(context);
    }
}

void CustomShelfFilter::updateShelfFilters(std::vector<float>& newFilterGains)
{
    //the new gain values for each filter are loaded from the APVTS
    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        auto shelfCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(_sampleRate, _shelfFrequency,
            _shelfQuality, juce::Decibels::decibelsToGain(newFilterGains[channel]));

        updateFilterCoefficients(_shelfFilters[channel]->coefficients, shelfCoefficients);
    }
}


void CustomShelfFilter::updateFilterCoefficients(Coefficients& old, Coefficients& replacements)
{
    //coefficients are updated
    *old = *replacements;
}