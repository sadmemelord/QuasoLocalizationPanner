/*
  ==============================================================================

    ComponentAttachment.h
    Created: 3 May 2023 6:43:23pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include<JuceHeader.h>

JUCE_API class ComponentAttachment
{
public:
    ComponentAttachment(juce::AudioProcessorValueTreeState& stateToUse,
                        const juce::String& parameterID,
                        juce::Component& component);

private:
    std::unique_ptr<juce::ParameterAttachment> attachment;
};