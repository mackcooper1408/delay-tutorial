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

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + " db";
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value)) + " %";
}

static juce::String stringFromMilliseconds(float value, int)
{
    if (value < 10.0f) {
        return juce::String(value, 2) + " ms";
    } else if (value < 100.0f) {
        return juce::String(value, 1) + " ms";
    } else if (value < 1000.0f) {
        return juce::String(int(value)) + " ms";
    } else {
        return juce::String(value * 0.001f, 2) + " s";
    }
}

static float millisecondsFromString(const juce::String& text)
{
    float value = text.getFloatValue();
    
    if (!text.endsWithIgnoreCase("ms")) {
        if (text.endsWithIgnoreCase("s") || value < Parameters::minDelayTime) {
            return value * 1000.0f;
        }
    }
    
    return value;
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamId.getParamID(), gainParam);
    castParameter(apvts, delayTimeParamId.getParamID(), delayTimeParam);
    castParameter(apvts, mixParamId.getParamID(), mixParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           gainParamId,
                                                           "Output gain",
                                                           juce::NormalisableRange<float> {-12.0f, 12.0f},
                                                           0.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           delayTimeParamId,
                                                           "Delay time",
                                                           juce::NormalisableRange<float> {minDelayTime, maxDelayTime, 0.001f, 0.25f},
                                                           100.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                                .withStringFromValueFunction(stringFromMilliseconds)
                                                                .withValueFromStringFunction(millisecondsFromString)));
    layout.add(std::make_unique<juce::AudioParameterFloat>(mixParamId,
                                                           "Mix",
                                                           juce::NormalisableRange<float> {0.0f, 100.0f, 1.0f},
                                                           100.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)));
    
    return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    mixSmoother.reset(sampleRate, duration);
    
    coeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    delayTime = 0.0f;
    mix = 1.0f;
    
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    
    targetDelayTime = delayTimeParam->get();
    if (targetDelayTime == 0.0f)
        delayTime = targetDelayTime;
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    mix = mixSmoother.getNextValue();
    delayTime += (targetDelayTime - delayTime) * coeff;
}
