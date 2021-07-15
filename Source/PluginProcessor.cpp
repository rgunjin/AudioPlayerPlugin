/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyPlug_inPlayerAudioProcessor::MyPlug_inPlayerAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output",
                                                    juce::AudioChannelSet::stereo())),
    mState (Stopped),
    mThumbnailCache (3),
    mWaveThumbnail (512, mFormatManager, mThumbnailCache),
    mPlayHead (mTransportSource)
{
    mFormatManager.registerBasicFormats();
    mTransportSource.addChangeListener (this);
}

MyPlug_inPlayerAudioProcessor::~MyPlug_inPlayerAudioProcessor()
{
}

//==============================================================================
const juce::String MyPlug_inPlayerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyPlug_inPlayerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyPlug_inPlayerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyPlug_inPlayerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyPlug_inPlayerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyPlug_inPlayerAudioProcessor::getNumPrograms()
{
    return 1;
}

int MyPlug_inPlayerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyPlug_inPlayerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MyPlug_inPlayerAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyPlug_inPlayerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyPlug_inPlayerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mTransportSource.prepareToPlay (samplesPerBlock, sampleRate);
}

void MyPlug_inPlayerAudioProcessor::releaseResources()
{
    mTransportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyPlug_inPlayerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MyPlug_inPlayerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    mTransportSource.getNextAudioBlock (juce::AudioSourceChannelInfo(buffer));
}

//==============================================================================
bool MyPlug_inPlayerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MyPlug_inPlayerAudioProcessor::createEditor()
{
    return new MyPlug_inPlayerAudioProcessorEditor (*this);
}

//==============================================================================
void MyPlug_inPlayerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void MyPlug_inPlayerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
void MyPlug_inPlayerAudioProcessor::loadFile (juce::File &file)
{
    auto* reader = mFormatManager.createReaderFor (file);

    if (reader != nullptr)
    {
        std::unique_ptr<juce::AudioFormatReaderSource> mNewSource (new juce::AudioFormatReaderSource (reader, true));
        mTransportSource.setSource (mNewSource.get(), 0, nullptr, reader->sampleRate);
        mWaveThumbnail.setFile (file);
        mReaderSource.reset(mNewSource.release());
    }
}

void MyPlug_inPlayerAudioProcessor::changeState (TransportState newState)
{
    if (mState != newState)
    {
        mState = newState;
        switch (mState)
        {
            case Stopped:
                mTransportSource.setPosition (0.0);
                break;

            case Starting:
                mTransportSource.start();
                break;

            case Pausing:
                mTransportSource.stop();
                break;

            case Paused:
                break;

            case Stopping:
                mTransportSource.stop();
                break;
        }
    }
}

void MyPlug_inPlayerAudioProcessor::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == &mTransportSource)
    {
        if (mTransportSource.isPlaying())
            changeState (Playing);
        else if ((mState == Stopping) || (mState == Playing))
            changeState (Stopped);
        else if (mState == Pausing)
            changeState (Paused);
    }
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyPlug_inPlayerAudioProcessor();
}
