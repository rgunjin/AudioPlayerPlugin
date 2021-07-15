/*
  ==============================================================================

    WaveThumbnail.h
    Created: 3 Jul 2021 3:57:36pm
    Author:  ruslan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveThumbnail  : public juce::Component,
                       private juce::ChangeListener
{
public:
    WaveThumbnail (int sourceSamplesPerThumbnailSamles,
                   juce::AudioFormatManager& formatManager,
                   juce::AudioThumbnailCache& cache);
    ~WaveThumbnail() override;

//==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

//==============================================================================
    void setFile (juce::File& file);

//==============================================================================
    void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);

//==============================================================================
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;

private:
    juce::AudioThumbnail thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
