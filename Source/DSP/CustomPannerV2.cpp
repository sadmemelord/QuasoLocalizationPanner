/*
  ==============================================================================

    CustomPannerV2.cpp
    Created: 11 Feb 2023 3:08:12pm
    Author:  Utente

  ==============================================================================
*/

#include "CustomPannerV2.h"



CustomPannerV2::CustomPannerV2()
{
    reset();
}

void CustomPannerV2::setPan(std::vector<float>& newPans, std::vector<bool>& newActiveTracks)
{
    //the new pan values coming from the apvts parameters are saved in a vector
    for (int bus = 0; bus < _busNumber; ++bus)
    {
        _pans[bus]= juce::jlimit(static_cast<float> (-1.0), static_cast<float> (1.0), newPans[bus]);
        _activeTracks[bus] = newActiveTracks[bus];
    }

    update();
}
void CustomPannerV2::prepare(const juce::dsp::ProcessSpec& spec)
{
    jassert(spec.sampleRate > 0);
    jassert(spec.numChannels > 0);

    _sampleRate = spec.sampleRate;

    reset();
}
void CustomPannerV2::reset()
{
    //every vector is filled with default values up to the number of input channels defined by _busNumber
    //the ramp for every smoothed value is also being reset
    for (int bus = 0; bus < _busNumber; ++bus)
    {
        _pans.push_back(0.0f);
        _leftVolumes.push_back(0.0f);
        _rightVolumes.push_back(0.0f);
        _leftVolumes[bus].reset(_sampleRate, 0.02);
        _rightVolumes[bus].reset(_sampleRate, 0.02);

        _activeTracks.push_back(true);
    }
}

void CustomPannerV2::update()
{
    float leftValue;
    float rightValue;
    float boostValue;

    for (int bus = 0; bus < _busNumber; ++bus)
    {
        auto normalisedPan = static_cast<float> (0.5) * (_pans[bus] + static_cast<float> (1.0));

        //the balanced panning method is applied for each bus
        leftValue = juce::jmin(static_cast<float> (0.5), static_cast<float> (1.0) - normalisedPan);
        rightValue = juce::jmin(static_cast<float> (0.5), normalisedPan);
        boostValue = static_cast<float> (2.0);

        _leftVolumes[bus].setTargetValue(leftValue * boostValue);
        _rightVolumes[bus].setTargetValue(rightValue * boostValue);

    }

}