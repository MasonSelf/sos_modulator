/*
  ==============================================================================

    SOSToggleButton.cpp
    Created: 17 May 2023 9:50:05am
    Author:  Mason Self

  ==============================================================================
*/

#include "SOSToggleButton.h"

SOSToggleButton::SOSToggleButton(IAudioProcessor& p, const juce::Identifier& paramID ,int paramIndex, const juce::String& textToDisplay)
    : Button(textToDisplay), audioProcessor(p)
{
    setToggleable(true);
    setClickingTogglesState(true);
    
    buttonAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.GetAPVTS(), paramID.toString(), *this);

    auto param = std::make_unique<juce::AudioParameterBool*>(static_cast<juce::AudioParameterBool*>(audioProcessor.GetParams().getUnchecked(paramIndex)));

    setToggleState((*param)->get(), juce::dontSendNotification);
}

void SOSToggleButton::paintButton (juce::Graphics& g, bool, bool)
{
    getToggleState() ?
        g.fillAll(juce::Colours::yellow) : g.fillAll(juce::Colours::lightgrey);
    g.setColour(juce::Colours::black);
    g.setFont(15.0f);
    g.drawFittedText(getName(), 0, 0, getWidth(), getHeight(), juce::Justification::centred, 1);
}


