/*
  ==============================================================================

    LevelMeter.h
    Created: 11 Aug 2024 12:46:01pm
    Author:  Mack Cooper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LevelMeter  : public juce::Component, private juce::Timer
{
public:
    LevelMeter(std::atomic<float>& measurementL, std::atomic<float>& measurementR);
    ~LevelMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    
    int positionForLevel(float dbLevel) const noexcept
    {
        return int(std::round(juce::jmap(dbLevel, maxdB, mindB, maxPos, minPos)));
    }
    
    void drawLevel(juce::Graphics& g, float level, int x, int width);
    
    std::atomic<float>& measurementL;
    std::atomic<float>& measurementR;
    
    float maxPos = 0.0f;
    float minPos = 0.0f;
    float dbLevelL;
    float dbLevelR;
    
    static constexpr float maxdB = 6.0f;
    static constexpr float mindB = -60.0f;
    static constexpr float stepdB = 6.0f;
    static constexpr float clampdB = -120.0f;
    static constexpr float clampLevel = 0.000001f; // -120 dB
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
