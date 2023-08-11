/*
  ==============================================================================

    SOSToggleButton.cpp
    Created: 17 May 2023 9:50:05am
    Author:  Mason Self

  ==============================================================================
*/

#include "SOSToggleButton.h"

SOSToggleButton::SOSToggleButton(IAudioProcessor& p,
                                 const juce::Identifier& paramID ,
                                 int paramIndex,
                                 const juce::String& textToDisplay,
                                 juce::Colour _offColor,
                                 juce::Colour _onColor,
                                 juce::Colour _textColor,
                                 juce::Colour _outlineColor)
    : Button(textToDisplay), audioProcessor(p), offColor(_offColor), onColor(_onColor), textColor(_textColor), outlineColor(_outlineColor)
{
    setToggleable(true);
    setClickingTogglesState(true);
    
    buttonAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.GetAPVTS(), paramID.toString(), *this);

    auto param = std::make_unique<juce::AudioParameterBool*>(static_cast<juce::AudioParameterBool*>(audioProcessor.GetParams().getUnchecked(paramIndex)));

    setToggleState((*param)->get(), juce::dontSendNotification);
}

void SOSToggleButton::paintButton (juce::Graphics& g, bool, bool) {


    if (getToggleState())
    {
        g.setColour(onColor);
        g.fillAll();
        g.setColour(outlineColor);
        juce::Path p;
        auto strokeThickness = 1.0f;
        p.startNewSubPath(0.0f, 0.0f);
        p.lineTo(getWidth(), 0.0f);
        p.lineTo(getWidth(), getHeight());
        p.lineTo(0.0f, getHeight());
        p.closeSubPath();
        g.strokePath(p, juce::PathStrokeType(strokeThickness));
    }
    else
    {
        auto shadowThickness = 4.0f;
        auto halfShadowThickness = shadowThickness / 2.0f;

        g.setColour(offColor);
        juce::Rectangle<float> buttonSurface{halfShadowThickness, halfShadowThickness,
                                             static_cast<float>(getWidth()) - shadowThickness,
                                             static_cast<float>(getHeight() - shadowThickness)};
        g.fillRect(buttonSurface);

        g.setColour(outlineColor);
        juce::Point<float> topRight{static_cast<float>(getWidth() - shadowThickness), 0.0f};
        juce::Point<float> bottomLeft{0.0f, static_cast<float>(getHeight()) - shadowThickness};
        {
            juce::Path p;
            p.startNewSubPath(0.0f, 0.0f);
            p.lineTo(topRight);
            g.strokePath(p, juce::PathStrokeType(shadowThickness));
        }
        {
            juce::Path p;
            p.startNewSubPath(0.0f, 0.0f);
            p.lineTo(bottomLeft);
            g.strokePath(p, juce::PathStrokeType(shadowThickness));
        }
        {
            juce::Path p;
            p.startNewSubPath(bottomLeft);
            p.lineTo(bottomLeft.translated(halfShadowThickness, 0.0f));
            p.lineTo(bottomLeft.translated(halfShadowThickness, halfShadowThickness));
            p.closeSubPath();
            g.fillPath(p);
        }
        {
            juce::Path p;
            p.startNewSubPath(topRight);
            p.lineTo(topRight.translated(0.0f, halfShadowThickness));
            p.lineTo(topRight.translated(halfShadowThickness, halfShadowThickness));
            p.closeSubPath();
            g.fillPath(p);
        }
    }
    if (getToggleState())
    {
        g.setColour(textColor);
    }
    else
    {
        g.setColour(textColor.interpolatedWith(juce::Colours::lightgrey, 0.35f));
    }
    g.setFont(15.0f);
    g.drawFittedText(getName(), 0, 0, getWidth(), getHeight(), juce::Justification::centred, 1);
}


