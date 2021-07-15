/*
  ==============================================================================

    PlayHead.cpp
    Created: 7 Jul 2021 7:16:42pm
    Author:  ruslan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayHead.h"

//==============================================================================
PlayHead::PlayHead(juce::AudioTransportSource& transportSourceToUse)
    : transportSource (transportSourceToUse)
{
    startTimer (40);
}

PlayHead::~PlayHead()
{
}

void PlayHead::paint (juce::Graphics& g)
{
    auto duration = (float) transportSource.getLengthInSeconds();

    if (duration > 0.0)
    {
        auto audioPosition = (float) transportSource.getCurrentPosition();
        auto drawPosition = (audioPosition / duration) * (float) getWidth();

        g.setColour (juce::Colours::green);
        g.drawLine (drawPosition, 0.0f, drawPosition, (float) getHeight(), 2.0f);
    }
}

void PlayHead::resized()
{
}

void PlayHead::mouseDown (const juce::MouseEvent& event)
{
    auto duration = (float) transportSource.getLengthInSeconds();
    
    if (duration > 0.0)
    {
        auto clickPosition = event.position.x;
        auto audioPosition = (clickPosition / (float) getWidth()) * duration;

        transportSource.setPosition (audioPosition);
    }
}

void PlayHead::timerCallback()
{
    repaint();
}
