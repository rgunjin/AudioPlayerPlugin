/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 3 Jul 2021 3:57:36pm
    Author:  ruslan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(int sourceSamplesPerThumbnailSamles,
                             juce::AudioFormatManager& formatManager,
                             juce::AudioThumbnailCache& cache)
    : thumbnail (sourceSamplesPerThumbnailSamles, formatManager, cache)
{
    thumbnail.addChangeListener (this);
}

WaveThumbnail::~WaveThumbnail()
{
}

//==============================================================================
void WaveThumbnail::paint (juce::Graphics& g)
{
    juce::Rectangle<int> thumbnailBounds (10, 10, getWidth() - 20, getHeight() - 15);

    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded (g, getLocalBounds());
    else
        paintIfFileLoaded (g, getLocalBounds());
}

void WaveThumbnail::resized()
{
}

void WaveThumbnail::paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour (juce::Colours::darkgrey);
    g.fillRect (thumbnailBounds);

    g.setColour (juce::Colours::white);
    g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

void WaveThumbnail::paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour (juce::Colours::darkgrey);
    g.fillRect (thumbnailBounds);

    g.setColour (juce::Colours::yellow);
    thumbnail.drawChannel (g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 0, 1.0f);
}
//==============================================================================
void WaveThumbnail::setFile (juce::File& file)
{
    thumbnail.setSource (new juce::FileInputSource (file));
}

void WaveThumbnail::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
        repaint();
}
