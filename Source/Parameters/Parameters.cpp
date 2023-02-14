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

juce::StringArray MultitrackPannerAudioProcessor::getGainIDs()
{
    juce::StringArray gIDs;

    for (int bus = 0; bus < getTotalNumInputChannels(); ++bus)
    {
        juce::String id = "gain";
        id << (bus + 1);

        gIDs.add(id);
    }

    return gIDs;
}

juce::StringArray MultitrackPannerAudioProcessor::getGainNames()
{
    juce::StringArray gNames;

    for (int bus = 0; bus < getTotalNumInputChannels(); ++bus)
    {
        juce::String name = "Gain ";
        name << (bus + 1);

        gNames.add(name);
    }

    return gNames;
}

