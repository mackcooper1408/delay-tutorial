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
    castParameter(apvts, delayTimeParamId.getParamID(), delayTimeParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
       gainParamId,
       "Output gain",
       juce::NormalisableRange<float> {-12.0f, 12.0f},
       0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
       delayTimeParamId,
       "Delay time",
       juce::NormalisableRange<float> {minDelayTime, maxDelayTime},
       100.0f));
    
    return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    delayTime = 0.0f;
    
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    delayTime = delayTimeParam->get();
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
}
