/*
  ==============================================================================

    Parameters.cpp
    Created: 5 Jul 2024 6:36:15pm
    Author:  Mack Cooper

  ==============================================================================
*/

#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& id, T& destination)
{
    
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamId.getParamID(), gainParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
       gainParamId,
       "Output gain",
       juce::NormalisableRange<float> {-12.0f, 12.0f},
       0.0f));
    
    return layout;
}

void Parameters::update() noexcept
{
    gain = juce::Decibels::decibelsToGain(gainParam->get());
}
