/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MyPlug_inPlayerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MyPlug_inPlayerAudioProcessorEditor (MyPlug_inPlayerAudioProcessor&);
    ~MyPlug_inPlayerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MyPlug_inPlayerAudioProcessor& audioProcessor;

    juce::TextButton mOpenButton;
    juce::TextButton mPlayButton;
    juce::TextButton mStopButton;

    void openButtonClick();
    void playButtonClick();
    void stopButtonClick();

    void addAudioFile();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyPlug_inPlayerAudioProcessorEditor)
};
