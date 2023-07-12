/*
  ==============================================================================

    SOSToggleButton.h
    Created: 17 May 2023 9:50:05am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <sos_IAudioProcessor/sos_IAudioProcessor.h>
#include <juce_gui_basics/juce_gui_basics.h>



class SOSToggleButton : public juce::Button
{
public:
    SOSToggleButton(IAudioProcessor& p, const juce::Identifier& paramID ,int paramIndex, const juce::String& textToDisplay);
    void paintButton (juce::Graphics &g, bool, bool) override;

private:
    IAudioProcessor& audioProcessor;
    
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
};
