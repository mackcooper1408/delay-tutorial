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

private:
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
};
