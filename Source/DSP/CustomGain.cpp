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
    for (int channel = 0; channel < _busNumber; ++channel)
    {
        _gains.push_back(0.0f);
    }
}

void CustomGain::setGainsDecibels(std::vector<float>& newGainsDecibels) noexcept
{
    //Applies a new gain as a decibel value

    for (int channel = 0; channel < _busNumber; ++channel)
    {
        _gains[channel].setTargetValue((juce::Decibels::decibelsToGain<float>(newGainsDecibels[channel])));
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
    for (int channel = 0; channel < _busNumber; ++channel)
    {

        _gains[channel].reset(_sampleRate, _rampDurationSeconds);
    }
}
