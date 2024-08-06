/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class DelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&);
    ~DelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayAudioProcessor& audioProcessor;
    
    MainLookAndFeel mainLF;
    
    RotaryKnob gainKnob { "Gain", audioProcessor.apvts, gainParamId, true };
    RotaryKnob mixKnob { "Mix", audioProcessor.apvts, mixParamId };
    RotaryKnob feedbackKnob { "Feedback", audioProcessor.apvts, feedbackParamId, true };
    RotaryKnob stereoKnob { "Stereo", audioProcessor.apvts, stereoParamId, true };
    RotaryKnob delayTimeKnob { "Time", audioProcessor.apvts, delayTimeParamId };
    RotaryKnob lowCutKnob { "Low Cut", audioProcessor.apvts, lowCutParamId };
    RotaryKnob highCutKnob { "High Cut", audioProcessor.apvts, highCutParamId };
    RotaryKnob delayNoteKnob { "Note", audioProcessor.apvts, delayNoteParamId };
    
    juce::TextButton tempoSyncButton;
    
    juce::AudioProcessorValueTreeState::ButtonAttachment tempoSyncAttachment {
        audioProcessor.apvts, tempoSyncParamId.getParamID(), tempoSyncButton
    };
    
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
