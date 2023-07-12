/*
  ==============================================================================

    CurveAdjusterLFO.h
    Created: 21 May 2023 9:52:45pm
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include "SOSToggleButton.h"
#include "LFO_modProcessor.h"
#include "ModulatorCurveAdjusterTrace.h"
#include "RateModesComponent.h"
#include "DestinationMatrixComponent.h"
#include "../ColorDefines_ShaveNFade.h"
#include <sliders_sos/sliders_sos.h>
#include <sos_curve_adjuster/sos_curve_adjuster.h>

class LFO_modEditor : public juce::Timer, public juce::Component
{
public:
    LFO_modEditor(AudioPluginAudioProcessor& p, LFO_modProcessor& _lfoModProcessor,
                  float curveWidth, float curveHeight,
                  int resetParamIndex,
                  int freezeParamIndex,
                  int freeRateParamIndex,
                  int syncRateParamIndex,
                  int rateBlendParamIndex,
                  int ampParamIndex,
                  std::vector<juce::String> modulationSourceNames, std::vector<juce::String> modulationDestinationNames,
                  juce::Colour _textColor,
                  juce::Colour _freeColor,
                  juce::Colour _syncColor);
    
    ~LFO_modEditor() override;
    void paint(juce::Graphics& g) override;
    void timerCallback() override;
    void resized() override;
private:
    
    juce::String GetFreeRatePercentageString();
    juce::String GetSyncRatePercentageString();

    AudioPluginAudioProcessor& processor;
    LFO_modProcessor& lfoModProcessor;
    
    SOSToggleButton resetButton;
    SOSToggleButton freezeButton;
    
    CurveAdjusterEditor curve;
    DirectionComponent directionMode;
    ModulatorCurveAdjusterTrace trace;
    SOSRotaryFloatSlider freeRate;
    SOSRotaryChoiceSlider syncRate;
    SOSSliderHorizontal rateBlend;
    SOSSliderVertical ampSlider;
    
    DestinationMatrixComponent destinationMatrix;
    
    juce::Colour textColor {juce::Colours::black};
    juce::Colour freeColor {juce::Colours::red};
    juce::Colour syncColor {juce::Colours::blue};
    juce::Colour knobOutlineColor {juce::Colours::black};
    juce::Colour knobPointerColor {juce::Colours::black};

};
