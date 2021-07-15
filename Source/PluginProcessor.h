/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveThumbnail.h"
#include "PlayHead.h"

//==============================================================================
/**
*/
class MyPlug_inPlayerAudioProcessor  : public juce::AudioProcessor,
                                       public juce::ChangeListener
{
public:
    //==============================================================================
    MyPlug_inPlayerAudioProcessor();
    ~MyPlug_inPlayerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping        
    };    

    TransportState mState;

    WaveThumbnail mWaveThumbnail;
    PlayHead mPlayHead;

    //==============================================================================
    void loadFile (juce::File &file);
    void changeState (TransportState newState);

private:
    //==============================================================================
    juce::AudioFormatManager mFormatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> mReaderSource;
    juce::AudioTransportSource mTransportSource;

    juce::AudioThumbnailCache mThumbnailCache;

    void changeListenerCallback (juce::ChangeBroadcaster* source) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyPlug_inPlayerAudioProcessor)
};
