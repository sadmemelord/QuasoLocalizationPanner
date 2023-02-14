/*
  ==============================================================================

    Parameters.cpp
    Created: 10 Feb 2023 6:44:25pm
    Author:  Utente

  ==============================================================================
*/

#include "../PluginProcessor.h"

juce::StringArray MultitrackPannerAudioProcessor::getPanIDs()
{
    juce::StringArray pIDs;

    for (int bus = 0; bus < getTotalNumInputChannels(); ++bus)
    {
        juce::String id = "pan";
        id << (bus + 1);

        pIDs.add(id);
    }

    return pIDs;
}

juce::StringArray MultitrackPannerAudioProcessor::getPanNames()
{
    juce::StringArray pNames;

    for (int bus = 0; bus < getTotalNumInputChannels(); ++bus)
    {
        juce::String name = "Pan ";
        name << (bus + 1);

        pNames.add(name);
    }

    return pNames;
}

