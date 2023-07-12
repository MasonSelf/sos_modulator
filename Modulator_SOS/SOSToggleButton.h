/*
  ==============================================================================

    SOSToggleButton.h
    Created: 17 May 2023 9:50:05am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include "../PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>



class SOSToggleButton : public juce::Button
{
public:
    SOSToggleButton(AudioPluginAudioProcessor& p, const juce::Identifier& paramID ,int paramIndex, const juce::String& textToDisplay);
    void paintButton (juce::Graphics &g, bool, bool) override;

private:
    AudioPluginAudioProcessor& audioProcessor;
    
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
};
