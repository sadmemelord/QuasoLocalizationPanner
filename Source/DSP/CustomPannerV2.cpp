
#include "CustomPannerV2.h"

CustomPannerV2::CustomPannerV2()
{
    reset();
}

void CustomPannerV2::setPan(std::vector<float>& newPans, std::vector<bool>& newActiveTracks)
{
    //the new pan and active tracks values loaded from the apvts parameters are saved in a vector
    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        _pans[channel]= juce::jlimit(static_cast<float> (-1.0), static_cast<float> (1.0), newPans[channel]);
        _activeTracks[channel] = newActiveTracks[channel];
    }

    update();
}

void CustomPannerV2::prepare(const juce::dsp::ProcessSpec& spec)
{
    _sampleRate = spec.sampleRate;
    reset();
}

void CustomPannerV2::reset()
{
    //every vector is filled with default values up to the number of input channels defined by _inputChannels
    //and the ramp for each smoothed value is also being reset
    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        _pans.push_back(0.0f);
        _leftVolumes.push_back(0.0f);
        _rightVolumes.push_back(0.0f);
        _activeTracks.push_back(true);

        _leftVolumes[channel].reset(_sampleRate, 0.02);
        _rightVolumes[channel].reset(_sampleRate, 0.02);
    }
}

void CustomPannerV2::update()
{
    float leftValue;
    float rightValue;
    float boostValue;

    for (int channel = 0; channel < _inputChannels; ++channel)
    {
        auto normalisedPan = static_cast<float> (0.5) * (_pans[channel] + static_cast<float> (1.0));

        //the balanced panning method is applied for each input channel
        leftValue = juce::jmin(static_cast<float> (0.5), static_cast<float> (1.0) - normalisedPan);
        rightValue = juce::jmin(static_cast<float> (0.5), normalisedPan);
        boostValue = static_cast<float> (2.0);

        _leftVolumes[channel].setTargetValue(leftValue * boostValue);
        _rightVolumes[channel].setTargetValue(rightValue * boostValue);

    }

}