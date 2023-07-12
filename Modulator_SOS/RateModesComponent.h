/*
  ==============================================================================

    RateModesComponent.h
    Created: 26 May 2023 2:16:14pm
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include "SOSToggleButton.h"
#include "LFO_modProcessor.h"

class NonAutomatableArrowButton : public juce::Button
{
public:
    enum Direction {forwards, backwards, bidirectional, bounce};
    
    NonAutomatableArrowButton(juce::String name, Direction d);
    void paintButton (juce::Graphics& g, bool, bool) override;
    
private:
    const float lineThickness = 4.0f;
    const float arrowWidth = 8.0f;
    const float arrowHeight = 8.0f;
    Direction direction;
};


class DirectionComponent: public juce::Component, public juce::Button::Listener, public juce::Timer
{
public:
    DirectionComponent(LFO_modProcessor& p, juce::String componentName);
    ~DirectionComponent() override;
    void timerCallback() override;
    void resized() override;
    void buttonClicked (juce::Button* b) override;
    
    NonAutomatableArrowButton forwardsButton, backwardsButton, bidirectionalButton, bounceButton;
    
    LFO_modProcessor& processor;
    
private:
    void UpdateSelectionFromProcessor();
};
