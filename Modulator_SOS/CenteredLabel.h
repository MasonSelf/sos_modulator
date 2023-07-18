/*
  ==============================================================================

    CenteredLabel.h
    Created: 29 Mar 2023 10:16:16pm
    Author:  thegr

  ==============================================================================
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

struct CenteredLabel : public juce::Label
{

    CenteredLabel(const juce::String name, const juce::String displayText, juce::Colour _textColor, juce::Colour _backgroundColor = {
            juce::Colours::transparentBlack})
        : juce::Label(name, displayText), backgroundColor(_backgroundColor), textColor(_textColor)
    {
    }
    
    void paint(juce::Graphics& g) override
    {
        g.setColour(backgroundColor);
        g.fillAll();
        g.setFont(10.0f);
        g.setColour(textColor);
        g.drawFittedText(getText(), 0, 0, getWidth(), getHeight(), juce::Justification::centred, 1);
    }
    
    juce::Colour backgroundColor, textColor;
};
