/*
  ==============================================================================

    DestinationMatrixProcessor.h
    Created: 28 May 2023 9:35:18am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include "ModulationMatrixData.h"
#include <juce_audio_processors/juce_audio_processors.h>

class ModulationMatrixProcessor
{
public:
    ModulationMatrixProcessor(std::vector<juce::String> _destinationNames, std::vector<juce::String> _sourceNames);
    float GetModulationValue(const juce::String& destinationName);
    void SaveState(juce::AudioProcessorValueTreeState& stateToAppendTo);
    void LoadAndRemoveStateFromAPTVS(juce::ValueTree& apvtsTree);
    
    std::vector<juce::String> destinationNames, sourceNames;
    ModulationMatrixData modVals;
    
    juce::Atomic<bool> readyForGuiInit {false};
private:
    float ConvertModulationValueAsPercentage(int index);
    
    void SetState(juce::ValueTree& lfoTree);
    void RemoveThisCurveAdjusterTreeFromAPVTS(juce::ValueTree& apvtsTree, juce::ValueTree& matrixTree);
    
    const juce::Identifier value_string_as_ID {"value"};
    juce::String name {"modulation_matrix"};
    const juce::String modValCollectionChildName {"attenuversion_values"};
    const juce::String modValChildName {"attenuversion_value"};
};
