/*
  ==============================================================================

    CustomPanner.cpp
    Created: 10 Feb 2023 6:44:17pm
    Author:  Utente

  ==============================================================================
*/

#include "CustomPanner.h"

CustomPanner::CustomPanner()
{
    update();
    reset();
}

void CustomPanner::setPan(float newPan1, float newPan2)
{
    //jassert(newPan >= -1.0 && newPan <= 1.0);

    _pan1 = juce::jlimit(static_cast<float> (-1.0), static_cast<float> (1.0), newPan1);
    _pan2 = juce::jlimit(static_cast<float> (-1.0), static_cast<float> (1.0), newPan2);
    update();
}
void CustomPanner::prepare(const juce::dsp::ProcessSpec& spec)
{
    jassert(spec.sampleRate > 0);
    jassert(spec.numChannels > 0);

    _sampleRate = spec.sampleRate;

    reset();
}
void CustomPanner::reset()
{
    
}

void CustomPanner::update()
{
    float _leftValue;
    float _rightValue;
    float _boostValue;

    auto _normalisedPan = static_cast<float> (0.5) * (_pan1 + static_cast<float> (1.0));
    //balanced panning method
    _leftValue = juce::jmin(static_cast<float> (0.5), static_cast<float> (1.0) - _normalisedPan);
    _rightValue = juce::jmin(static_cast<float> (0.5), _normalisedPan);
    _boostValue = static_cast<float> (2.0);

    _leftVolume1 = (_leftValue * _boostValue);
    _rightVolume1 = (_rightValue * _boostValue);

   _normalisedPan = static_cast<float> (0.5) * (_pan2 + static_cast<float> (1.0));
    //balanced panning method
    _leftValue = juce::jmin(static_cast<float> (0.5), static_cast<float> (1.0) - _normalisedPan);
    _rightValue = juce::jmin(static_cast<float> (0.5), _normalisedPan);
    _boostValue = static_cast<float> (2.0);

    _leftVolume2 = (_leftValue * _boostValue);
    _rightVolume2 = (_rightValue * _boostValue);
}