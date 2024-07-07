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
    void update() noexcept;
    
    float gain = 0.0f;

private:
    juce::AudioParameterFloat* gainParam;
};
