/*
  ==============================================================================

    ModulationPopUpWindow.h
    Created: 28 May 2023 5:25:38pm
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include "../PopUpWindow_SOS/SOSPopUpWindowManager.h"
#include "../Modulator_SOS/LFO_modEditor.h"
#include "../PluginProcessor.h"
#include "../ColorDefines_ShaveNFade.h"

class ModulationPopUpWindow : public SOSPopUpWindowManager, public juce::Component, public juce::Value::Listener
{
public:
    ModulationPopUpWindow(AudioPluginAudioProcessor& p, LFO_modProcessor& _lfoModProcessor,
                          float curveWidth, float curveHeight,
                          int resetParamIndex,
                          int freezeParamIndex,
                          int freeRateParamIndex,
                          int syncRateParamIndex,
                          int rateBlendParamIndex,
                          int ampParamIndex,
                          std::vector<juce::String> modulationSourceNames, std::vector<juce::String> modulationDestinationNames);
    ~ModulationPopUpWindow() override;
    void resized() override;
    void valueChanged (juce::Value&) override;
private:
    LFO_modEditor modEditor;
};
