/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyPlug_inPlayerAudioProcessorEditor::MyPlug_inPlayerAudioProcessorEditor (MyPlug_inPlayerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (450, 150);

    addAndMakeVisible (mOpenButton);
    mOpenButton.setButtonText ("Open...");
    mOpenButton.setColour (mOpenButton.buttonColourId, juce::Colours::darkblue);
    mOpenButton.onClick = [this] { openButtonClick(); };

    addAndMakeVisible (mPlayButton);
    mPlayButton.setButtonText ("Play/Pause");
    mPlayButton.setColour (mPlayButton.buttonColourId, juce::Colours::darkgreen);
    mPlayButton.onClick = [this] { playButtonClick(); };
    mPlayButton.setEnabled (false);
    
    addAndMakeVisible (mStopButton);
    mStopButton.setButtonText ("Stop");
    mStopButton.setColour (mStopButton.buttonColourId, juce::Colours::maroon);
    mStopButton.onClick = [this] { stopButtonClick(); };
    mStopButton.setEnabled (false);

    addAndMakeVisible (audioProcessor.mWaveThumbnail);
    addAndMakeVisible (audioProcessor.mPlayHead);
}

MyPlug_inPlayerAudioProcessorEditor::~MyPlug_inPlayerAudioProcessorEditor()
{
}

//==============================================================================
void MyPlug_inPlayerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MyPlug_inPlayerAudioProcessorEditor::resized()
{
    mOpenButton.setBounds (10, getHeight() - 40, getWidth() / 3 - 10, 30);
    mPlayButton.setBounds (getWidth() / 3 + 10 , getHeight() - 40, getWidth() / 3 - 10, 30);
    mStopButton.setBounds (2 * (getWidth() / 3) + 10, getHeight() - 40, getWidth() / 3 - 17, 30);

    juce::Rectangle<int> thumbnailBounds (0, 0, getWidth(), getHeight() - 45);
    audioProcessor.mWaveThumbnail.setBounds (thumbnailBounds);
    audioProcessor.mPlayHead.setBounds (thumbnailBounds);
}

void MyPlug_inPlayerAudioProcessorEditor::openButtonClick()
{
    addAudioFile();
    mPlayButton.setEnabled(true);
}

void MyPlug_inPlayerAudioProcessorEditor::playButtonClick()
{
    if ((audioProcessor.mState == MyPlug_inPlayerAudioProcessor::TransportState::Stopped) || (audioProcessor.mState == MyPlug_inPlayerAudioProcessor::TransportState::Paused))
    audioProcessor.changeState (MyPlug_inPlayerAudioProcessor::TransportState::Starting);
    else if (audioProcessor.mState == MyPlug_inPlayerAudioProcessor::TransportState::Playing)
        audioProcessor.changeState (MyPlug_inPlayerAudioProcessor::TransportState::Pausing);
    mOpenButton.setEnabled (false);
    mStopButton.setEnabled (true);
}


void MyPlug_inPlayerAudioProcessorEditor::stopButtonClick()
{
    if (audioProcessor.mState == MyPlug_inPlayerAudioProcessor::TransportState::Paused)
        audioProcessor.changeState (MyPlug_inPlayerAudioProcessor::TransportState::Stopped);
    else
        audioProcessor.changeState (MyPlug_inPlayerAudioProcessor::TransportState::Stopping);
    mOpenButton.setEnabled (true);
}

void MyPlug_inPlayerAudioProcessorEditor::addAudioFile()
{
    juce::FileChooser chooser ("Select audio file...", juce::File::getSpecialLocation(juce::File::userMusicDirectory), "*.wav; *.mp3");

    if (chooser.browseForFileToOpen())
    {
        juce::File file (chooser.getResult());
        audioProcessor.loadFile(file);
    }
}
