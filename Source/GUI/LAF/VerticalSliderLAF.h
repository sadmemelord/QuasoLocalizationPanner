/*
  ==============================================================================

    VerticalSliderLAF.h
    Created: 30 Apr 2023 6:21:28pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class VerticalSliderLAF: public juce::LookAndFeel_V4
{

public:
    VerticalSliderLAF();
    ~VerticalSliderLAF();


    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                            const juce::Slider::SliderStyle style, juce::Slider& slider) override;

private:

};

