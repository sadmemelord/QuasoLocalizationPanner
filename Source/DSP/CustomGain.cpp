/*
  ==============================================================================

    CustomGain.cpp
    Created: 14 Feb 2023 5:36:11pm
    Author:  Utente

  ==============================================================================
*/

#include "CustomGain.h"

CustomGain::CustomGain()
{
    for (int bus = 0; bus < _busNumber; ++bus)
    {
        _gains.push_back(0.0f);
    }
}

void CustomGain::setGainsDecibels(std::vector<float>& newGainsDecibels) noexcept
{
    //Applies a new gain as a decibel value

    for (int bus = 0; bus < _busNumber; ++bus)
    {
        _gains[bus].setTargetValue((juce::Decibels::decibelsToGain<float>(newGainsDecibels[bus])));
    }
}

void CustomGain::setRampDurationSeconds(double newDurationSeconds) noexcept
{
    //Sets the length of the ramp used for smoothing gain changes
    if (_rampDurationSeconds != newDurationSeconds)
    {
        _rampDurationSeconds = newDurationSeconds;
        reset();
    }
}

void CustomGain::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    //Called before processing starts
    _sampleRate = spec.sampleRate;
    reset();
}

void CustomGain::reset() noexcept
{
    //Resets the internal state of the gain
    for (int bus = 0; bus < _busNumber; ++bus)
    {

        _gains[bus].reset(_sampleRate, _rampDurationSeconds);
    }
}
