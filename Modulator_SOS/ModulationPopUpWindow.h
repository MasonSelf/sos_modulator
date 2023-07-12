/*
  ==============================================================================

    ModulationPopUpWindow.h
    Created: 28 May 2023 5:25:38pm
    Author:  Mason Self

  ==============================================================================
*/

#pragma once

#include "../Modulator_SOS/LFO_modEditor.h"
#include <sos_popup_window/sos_popup_window.h>
#include <sos_IAudioProcessor/sos_IAudioProcessor.h>


class ModulationPopUpWindow : public SOSPopUpWindowManager, public juce::Component, public juce::Value::Listener
{
public:
    ModulationPopUpWindow(IAudioProcessor& p, LFO_modProcessor& _lfoModProcessor,
                          float curveWidth, float curveHeight,
                          int resetParamIndex,
                          int freezeParamIndex,
                          int freeRateParamIndex,
                          int syncRateParamIndex,
                          int rateBlendParamIndex,
                          int ampParamIndex,
                          std::vector<juce::String> modulationSourceNames, std::vector<juce::String> modulationDestinationNames,
                          juce::Colour backgroundColor,
                          juce::Colour textColor,
                          juce::Colour syncRateColor,
                          juce::Colour freeRateColor);
    ~ModulationPopUpWindow() override;
    void resized() override;
    void valueChanged (juce::Value&) override;
private:
    LFO_modEditor modEditor;

};
