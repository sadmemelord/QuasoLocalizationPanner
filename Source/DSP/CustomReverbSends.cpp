
#include "CustomReverbSends.h"

CustomReverbSends::CustomReverbSends()
{
    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        _reverbSendsGains.push_back(0.0f);
    }
}
void CustomReverbSends::setReverbSendsGains(std::vector<float>& distance)
{
    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        //the Distance parameter is stored as a value between 0 and 1 where 0 represents the furthest 
        //the reverb send level is greater when the source is further and so we need to map the distance
        //interval on a new one from 1 to 0.
        float _currentDistance = distance[channel];
        float _remappedDistance = 1.0f - _currentDistance;

        _reverbSendsGains[channel].setTargetValue(juce::Decibels::decibelsToGain<float>(juce::jmap(_remappedDistance, -60.0f, -10.0f)));
      
    }
}

void CustomReverbSends::setRampDurationSeconds(double newDurationSeconds) noexcept
{
    //Sets the length of the ramp used for smoothing gain changes
    if (_rampDurationSeconds != newDurationSeconds)
    {
        _rampDurationSeconds = newDurationSeconds;
        reset();
    }
}

void CustomReverbSends::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    //Called before processing starts
    _sampleRate = spec.sampleRate;
    reset();
}

void CustomReverbSends::reset() noexcept
{
    //Resets the internal state of the gain
    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        _reverbSendsGains[channel].reset(_sampleRate, _rampDurationSeconds);
    }
}