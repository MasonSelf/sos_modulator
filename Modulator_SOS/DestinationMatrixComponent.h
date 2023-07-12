/*
  ==============================================================================

    DestinationMatrix.h
    Created: 27 May 2023 1:04:55am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include "DestinationAttenuverter.h"
#include "ModulationMatrixProcessor.h"
#include "CenteredLabel.h"
#include <juce_gui_basics/juce_gui_basics.h>

class DestinationMatrixComponent : public juce::Component, public juce::Value::Listener, public juce::Timer
{
public:
    DestinationMatrixComponent(std::vector<juce::String> destinations, std::vector<juce::String> sources, ModulationMatrixProcessor& p, juce::Colour _textColor);
    ~DestinationMatrixComponent() override;
    void resized() override;
    void valueChanged(juce::Value& v) override;
    void timerCallback() override;
    
    std::vector<std::unique_ptr<DestinationAttenuverter>> attenuverterComponents;
    
    ModulationMatrixProcessor& processor;
    
private:
    std::vector<std::unique_ptr<CenteredLabel>> destinationNames, sourceNames;
    juce::Colour textColor;
};
