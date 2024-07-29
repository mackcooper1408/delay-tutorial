/*
  ==============================================================================

    Parameters.h
    Created: 5 Jul 2024 6:36:15pm
    Author:  Mack Cooper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

const juce::ParameterID gainParamId { "gain", 1 };
const juce::ParameterID delayTimeParamId { "delayTime", 1 };
const juce::ParameterID mixParamId { "mix", 1 };
const juce::ParameterID feedbackParamId { "feedback", 1 };
const juce::ParameterID stereoParamId { "stereo", 1};

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
    
    
    float gain = 0.0f;
    float delayTime = 0.0f;
    float mix = 1.0f;
    float feedback = 0.0f;
    
    float panL = 0.0f;
    float panR = 0.0f;

private:
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;

    juce::AudioParameterFloat* delayTimeParam;
    float targetDelayTime = 0.0f;
    float coeff = 0.0f;
    
    juce::AudioParameterFloat* mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;
    
    juce::AudioParameterFloat* feedbackParam;
    juce::LinearSmoothedValue<float> feedbackSmoother;
    
    juce::AudioParameterFloat* stereoParam;
    juce::LinearSmoothedValue<float> stereoSmoother;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameters)
};
