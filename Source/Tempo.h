/*
  ==============================================================================

    Tempo.h
    Created: 7 Aug 2024 12:32:51pm
    Author:  Mack Cooper

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Tempo
{
public:
    void reset() noexcept;
    
    void update(const juce::AudioPlayHead* playhead) noexcept;
    
    double getMillisecondsForNoteLength(int index) const noexcept;
    
    double getTempo() const noexcept
    {
        return bpm;
    }
private:
    double bpm = 120.0;
};
