/*
 ==============================================================================
 
 Parameters.cpp
 Created: 5 Jul 2024 6:36:15pm
 Author:  Mack Cooper
 
 ==============================================================================
 */

#include "Parameters.h"
#include "DSP.h"

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

static juce::String stringFromHz(float value, int)
{
    if (value < 1000.0f) {
        return juce::String(int(value)) + " Hz";
    } else if (value < 10000.0f) {
        return juce::String(value / 1000.0f, 2) + " k";
    } else {
        return juce::String(value / 1000.0f, 1) + " k";
    }
}

static float hzFromString(const juce::String& str)
{
    float value = str.getFloatValue();
    if (value < 20.0f) {
        return value * 1000.0f;
    }
    return value;
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamId.getParamID(), gainParam);
    castParameter(apvts, delayTimeParamId.getParamID(), delayTimeParam);
    castParameter(apvts, tempoSyncParamId.getParamID(), tempoSyncParam);
    castParameter(apvts, delayNoteParamId.getParamID(), delayNoteParam);
    castParameter(apvts, mixParamId.getParamID(), mixParam);
    castParameter(apvts, feedbackParamId.getParamID(), feedbackParam);
    castParameter(apvts, stereoParamId.getParamID(), stereoParam);
    castParameter(apvts, lowCutParamId.getParamID(), lowCutParam);
    castParameter(apvts, highCutParamId.getParamID(), highCutParam);
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
    layout.add(std::make_unique<juce::AudioParameterFloat>(feedbackParamId,
                                                           "Feedback",
                                                           juce::NormalisableRange<float> {-100.0f, 100.0f, 1.0f},
                                                           0.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)));
    layout.add(std::make_unique<juce::AudioParameterFloat>(stereoParamId,
                                                           "Stereo",
                                                           juce::NormalisableRange<float> {-100.0f, 100.0f, 1.0f},
                                                           0.0f,
                                                           juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)));
    layout.add(std::make_unique<juce::AudioParameterFloat>(lowCutParamId,
                                                           "Low Cut",
                                                           juce::NormalisableRange<float> {20.0f, 20000.0f, 0.3f},
                                                           20.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                                .withStringFromValueFunction(stringFromHz)
                                                                .withValueFromStringFunction(hzFromString)));
    layout.add(std::make_unique<juce::AudioParameterFloat>(highCutParamId,
                                                           "High Cut",
                                                           juce::NormalisableRange<float> {20.0f, 20000.0f, 0.3f},
                                                           20000.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                                .withStringFromValueFunction(stringFromHz)
                                                                .withValueFromStringFunction(hzFromString)));
    layout.add(std::make_unique<juce::AudioParameterBool>(tempoSyncParamId, "Tempo Sync", false));
    
    juce::StringArray noteLengths = {
        "1/32",
        "1/16 trip",
        "1/32 dot",
        "1/16",
        "1/8 trip",
        "1/16 dot",
        "1/8",
        "1/4 trip",
        "1/8 dot",
        "1/4",
        "1/2 trip",
        "1/4 dot",
        "1/2",
        "1/1 trip",
        "1/2 dot",
        "1/1",
    };
    layout.add(std::make_unique<juce::AudioParameterChoice>(delayNoteParamId, "Delay Note", noteLengths, 9));
    
    return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    mixSmoother.reset(sampleRate, duration);
    feedbackSmoother.reset(sampleRate, duration);
    stereoSmoother.reset(sampleRate, duration);
    lowCutSmoother.reset(sampleRate, duration);
    highCutSmoother.reset(sampleRate, duration);
    
    coeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    delayTime = 0.0f;
    mix = 1.0f;
    feedback = 0.0f;
    
    panL = 0.0f;
    panR = 0.0f;
    
    lowCut = 20.0f;
    highCut = 20000.0f;
    
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);
    feedbackSmoother.setCurrentAndTargetValue(feedbackParam->get() * 0.01f);
    stereoSmoother.setCurrentAndTargetValue(stereoParam->get() * 0.01f);
    lowCutSmoother.setCurrentAndTargetValue(lowCutParam->get());
    highCutSmoother.setCurrentAndTargetValue(highCutParam->get());
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    feedbackSmoother.setTargetValue(feedbackParam->get() * 0.01f);
    stereoSmoother.setTargetValue(stereoParam->get() * 0.01f);
    lowCutSmoother.setTargetValue(lowCutParam->get());
    highCutSmoother.setTargetValue(highCutParam->get());
    
    targetDelayTime = delayTimeParam->get();
    if (targetDelayTime == 0.0f)
        delayTime = targetDelayTime;
    
    delayNote = delayNoteParam->getIndex();
    tempoSync = tempoSyncParam->get();
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    mix = mixSmoother.getNextValue();
    feedback = feedbackSmoother.getNextValue();
    delayTime += (targetDelayTime - delayTime) * coeff;
    
    panningEqualPower(stereoSmoother.getNextValue(), panL, panR);
    
    lowCut = lowCutSmoother.getNextValue();
    highCut = highCutSmoother.getNextValue();
}
