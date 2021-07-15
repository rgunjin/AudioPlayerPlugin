/*
  ==============================================================================

    PlayHead.h
    Created: 7 Jul 2021 7:16:42pm
    Author:  ruslan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlayHead  : public juce::Component,
                  private juce::Timer
{
public:
    PlayHead(juce::AudioTransportSource& transportSourceToUse);
    ~PlayHead() override;

//==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseDown (const juce::MouseEvent& event) override;

private:
    void timerCallback() override;

    juce::AudioTransportSource& transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayHead)
};
