/*
  ==============================================================================

    LevelMeter.cpp
    Created: 11 Aug 2024 12:46:01pm
    Author:  Mack Cooper

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LevelMeter.h"
#include "LookAndFeel.h"

//==============================================================================
LevelMeter::LevelMeter(std::atomic<float>& measurementL_, std::atomic<float>& measurementR_)
    : measurementL(measurementL_), measurementR(measurementR_), dbLevelL(clampdB), dbLevelR(clampdB)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setOpaque(true);
    startTimerHz(60);
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    
    g.fillAll(Colors::LevelMeter::background);
    
    g.setFont(Fonts::getFont(10.0f));
    
    for (auto db = maxdB; db >= mindB; db -= stepdB) {
        int y = positionForLevel(db);
        
        g.setColour(Colors::LevelMeter::tickLine);
        g.fillRect(0, y, 16, 1);
        
        g.setColour(Colors::LevelMeter::tickLabel);
        g.drawSingleLineText(juce::String(int(db)), bounds.getWidth(), y + 3, juce::Justification::right);
    }
    
    drawLevel(g, dbLevelL, 0, 7);
    drawLevel(g, dbLevelR, 9, 7);
}

void LevelMeter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    maxPos = 4.0f;
    minPos = float(getHeight() - 4.0f);
}

void LevelMeter::timerCallback()
{
    dbLevelL = juce::Decibels::gainToDecibels(measurementL.load(), clampdB);
    dbLevelR = juce::Decibels::gainToDecibels(measurementR.load(), clampdB);
    
    repaint();
}

void LevelMeter::drawLevel(juce::Graphics &g, float level, int x, int width)
{
    int y = positionForLevel(level);
    if (level > 0.0f) {
        int y0 = positionForLevel(0.0f);
        g.setColour(Colors::LevelMeter::tooLoud);
        g.fillRect(x, y, width, y0 - y);
        g.setColour(Colors::LevelMeter::levelOk);
        g.fillRect(x, y0, width, getHeight() - y0);
    } else if (y < getHeight()) {
        g.setColour(Colors::LevelMeter::levelOk);
        g.fillRect(x, y, width, getHeight() - y);
    }
}
