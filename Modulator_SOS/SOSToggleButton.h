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
    SOSToggleButton(IAudioProcessor& p,
                    const juce::Identifier& paramID ,
                    int paramIndex,
                    const juce::String& textToDisplay,
                    juce::Colour _offColor = juce::Colours::lightgrey,
                    juce::Colour _onColor = juce::Colours::yellow,
                    juce::Colour _textColor = juce::Colours::black,
                    juce::Colour _outlineColor = juce::Colours::grey);
    void paintButton (juce::Graphics &g, bool, bool) override;

    bool cachedToggleState {false};
private:
    IAudioProcessor& audioProcessor;
    juce::Colour offColor, onColor, textColor, outlineColor;
    
public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
};
