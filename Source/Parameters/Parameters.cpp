
#include "../PluginProcessor.h"

//These methods return an array of strings containing ID and Name of every paramater (up to the number of input channels).
//every ID is written in the form of "name+number" while every Name is expressed in the form of "Name + number"
//for example the ID and Name of the Pan parameter of track 2 will be "pan2" and "Pan 2"

juce::StringArray QuasoLocalizationPannerAudioProcessor::getActiveTracksIDs()
{
    juce::StringArray tIDs;

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        juce::String id = "track";
        id << (channel + 1);

        tIDs.add(id);
    }

    return tIDs;

}

juce::StringArray QuasoLocalizationPannerAudioProcessor::getActiveTracksNames()
{
    juce::StringArray tNames;

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        juce::String name = "Track ";
        name << (channel + 1);

        tNames.add(name);
    }

    return tNames;
}


juce::StringArray QuasoLocalizationPannerAudioProcessor::getDistanceIDs()
{
    juce::StringArray dIDs;

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        juce::String id = "distance";
        id << (channel + 1);

        dIDs.add(id);
    }

    return dIDs;
}

juce::StringArray QuasoLocalizationPannerAudioProcessor::getDistanceNames()
{
    juce::StringArray dNames;

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        juce::String name = "Distance ";
        name << (channel + 1);

        dNames.add(name);
    }

    return dNames;
}

juce::StringArray QuasoLocalizationPannerAudioProcessor::getPanIDs()
{
    juce::StringArray pIDs;

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        juce::String id = "pan";
        id << (channel + 1);

        pIDs.add(id);
    }

    return pIDs;
}

juce::StringArray QuasoLocalizationPannerAudioProcessor::getPanNames()
{
    juce::StringArray pNames;

    for (int channel = 0; channel < INPUTCHANNELS; ++channel)
    {
        juce::String name = "Pan ";
        name << (channel + 1);

        pNames.add(name);
    }

    return pNames;
}

