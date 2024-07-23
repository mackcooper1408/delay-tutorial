/*
 ==============================================================================
 
 RotaryKnob.cpp
 Created: 11 Jul 2024 2:18:30pm
 Author:  Mack Cooper
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "LookAndFeel.h"

    //==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterId) : attachment(apvts, parameterId.getParamID(), slider)
{
    float pi = juce::MathConstants<float>::pi;
    slider.setRotaryParameters(1.25f * pi, 2.75 * pi, true);

    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    slider.setBounds(0, 0, 70, 86);
    addAndMakeVisible(slider);
    
    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>(0));
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
    
    setLookAndFeel(RotaryKnobLookAndFeel::get());
    
    setSize(70, 110);
}

RotaryKnob::~RotaryKnob()
{
}

void RotaryKnob::resized()
{
        // This method is where you should set the bounds of any child
        // components that your component contains..
    slider.setTopLeftPosition(0, 24);
}
