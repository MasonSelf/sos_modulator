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

#include <sliders_sos/sliders_sos.h>
#include <sos_curve_adjuster/sos_curve_adjuster.h>

class LFO_modEditor : public juce::Timer, public juce::Component
{
public:
    LFO_modEditor(IAudioProcessor& p, LFO_modProcessor& _lfoModProcessor,
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
    int GetDisplaySigFigures(float normalizedValue);

    //AudioPluginAudioProcessor& processor;
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

    //this needs to be destroyed BEFORE ampSlider because it takes references to ampSlider
    DestinationMatrixComponent destinationMatrix;

    juce::Colour trackColor {juce::Colour(180, 188, 194)};
    juce::Colour textColor {juce::Colours::black};
    juce::Colour freeColor {juce::Colours::red};
    juce::Colour syncColor {juce::Colours::blue};


};
