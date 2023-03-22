/*
  ==============================================================================

    Parameters.cpp
    Created: 10 Feb 2023 6:44:25pm
    Author:  Utente

  ==============================================================================
*/

#include "../PluginProcessor.h"

//These methods return an array of strings containing ID and Name of every paramater up to the number of input channels.
//every ID is expressed in the form of "name+number" while every Name is expressed in the form of "Name + number"
juce::StringArray MultitrackPannerAudioProcessor::getPanIDs()
{
    juce::StringArray pIDs;

    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
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

    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
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

    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
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

    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
    {
        juce::String name = "Gain ";
        name << (bus + 1);

        gNames.add(name);
    }

    return gNames;
}
juce::StringArray MultitrackPannerAudioProcessor::getFilterGainIDs()
{
    juce::StringArray fIDs;

    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
    {
        juce::String id = "filter";
        id << (bus + 1);

        fIDs.add(id);
    }

    return fIDs;
}
juce::StringArray MultitrackPannerAudioProcessor::getFilterGainNames()
{
    juce::StringArray fNames;

    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
    {
        juce::String name = "Filter ";
        name << (bus + 1);

        fNames.add(name);
    }

    return fNames;
}
juce::StringArray MultitrackPannerAudioProcessor::getActiveTracksIDs()
{
    juce::StringArray tIDs;

    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
    {
        juce::String id = "track";
        id << (bus + 1);

        tIDs.add(id);
    }

    return tIDs;

}
juce::StringArray MultitrackPannerAudioProcessor::getActiveTracksNames()
{
    juce::StringArray tNames;

    for (int bus = 0; bus < getBusesLayout().inputBuses.size(); ++bus)
    {
        juce::String name = "Track ";
        name << (bus + 1);

        tNames.add(name);
    }

    return tNames;
}

