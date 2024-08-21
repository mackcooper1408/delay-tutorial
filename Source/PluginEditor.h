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
#include "LevelMeter.h"

//==============================================================================
/**
*/
class DelayAudioProcessorEditor : public juce::AudioProcessorEditor,
                                  private juce::AudioProcessorParameter::Listener
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&);
    ~DelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override { }
    void updateDelayKnobs(bool tempoSyncActive);

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
    
    LevelMeter meter;
    
    juce::TextButton tempoSyncButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment tempoSyncAttachment {
        audioProcessor.apvts, tempoSyncParamId.getParamID(), tempoSyncButton
    };
    
    juce::ImageButton bypassButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment {
        audioProcessor.apvts, bypassParamId.getParamID(), bypassButton
    };
    
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
