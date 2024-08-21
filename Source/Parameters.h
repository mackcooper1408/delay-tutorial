/*
  ==============================================================================

    Parameters.h
    Created: 5 Jul 2024 6:36:15pm
    Author:  Mack Cooper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

const juce::ParameterID delayTimeParamId { "delayTime", 1 };
const juce::ParameterID tempoSyncParamId { "tempoSync", 1 };
const juce::ParameterID delayNoteParamId { "delayNote", 1 };
const juce::ParameterID feedbackParamId { "feedback", 1 };
const juce::ParameterID stereoParamId { "stereo", 1};
const juce::ParameterID lowCutParamId { "lowCut", 1};
const juce::ParameterID highCutParamId { "highCut", 1};
const juce::ParameterID mixParamId { "mix", 1 };
const juce::ParameterID gainParamId { "gain", 1 };
const juce::ParameterID bypassParamId { "bypass", 1 };

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    static constexpr float minDelayTime = 5.0f;
    static constexpr float maxDelayTime = 5000.0f;
    
    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void smoothen() noexcept;
    
    
    float delayTime = 0.0f;
    
    float feedback = 0.0f;
    float panL = 0.0f;
    float panR = 0.0f;
    float lowCut = 20.0f;
    float highCut = 20000.0f;
    
    int delayNote = 0;
    bool tempoSync = false;

    float mix = 1.0f;
    float gain = 0.0f;
    
    juce::AudioParameterBool* bypassParam;
    bool bypassed = false;

    juce::AudioParameterBool* tempoSyncParam;
private:
    juce::AudioParameterFloat* delayTimeParam;
    float targetDelayTime = 0.0f;
    float coeff = 0.0f;
    
    juce::AudioParameterChoice* delayNoteParam;
    
    juce::AudioParameterFloat* feedbackParam;
    juce::LinearSmoothedValue<float> feedbackSmoother;
    
    juce::AudioParameterFloat* stereoParam;
    juce::LinearSmoothedValue<float> stereoSmoother;
    
    juce::AudioParameterFloat* lowCutParam;
    juce::LinearSmoothedValue<float> lowCutSmoother;
    
    juce::AudioParameterFloat* highCutParam;
    juce::LinearSmoothedValue<float> highCutSmoother;
    
    juce::AudioParameterFloat* mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;
    
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameters)
};
