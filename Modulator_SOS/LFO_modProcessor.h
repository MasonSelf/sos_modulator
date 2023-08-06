/*
  ==============================================================================

    LFO_Processor.h
    Created: 21 May 2023 10:03:51pm
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <sos_curve_adjuster/sos_curve_adjuster.h>
//#include "../CurveAdjuster_SOS/CurveAdjusterProcessor.h"
#include "SyncTiming.h"
//#include "../CurveAdjuster_SOS/DebugHelperFunctions.h"
#include "TempoDataHandler.h"
#include "ModulationMatrixProcessor.h"


class LFO_modProcessor
{
public:
    LFO_modProcessor(std::string n, juce::AudioProcessorValueTreeState& _apvts,
                     const juce::Identifier& rateFreeParamName,
                     const juce::Identifier& rateSyncParamName,
                     const juce::Identifier& rateBlendParamName,
                     const juce::Identifier& resetParamName,
                     const juce::Identifier& freezeParamName,
                     const juce::Identifier& scaleParamName,
                     std::vector<juce::String> modDestinations);
    
    void Init(double _sampleRate);
    void Process(int numSamples);
    void SaveState(juce::AudioProcessorValueTreeState& stateToAppendTo);
    void LoadAndRemoveStateFromAPTVS(juce::ValueTree& apvtsTree);
    
    float GetModdedVal(const juce::Identifier& paramID, const juce::String& destinationName);
    
    std::atomic<bool> readyForGuiUpdate {false};
    
    std::atomic<float> value; //scaled modulation value (prematrix)
    std::atomic<double> freePhase{0.0}, syncPhase{0.0};
    
    CurveAdjuster::CurveAdjusterProcessor curveAdjusterProcessor;
    
    const juce::Identifier rateFreeParamID, rateSyncParamID, rateBlendParamID;
    const juce::Identifier resetParamID;
    const juce::Identifier freezeParamID;
    const juce::Identifier scaleParamID;
    juce::AudioProcessorValueTreeState& apvts;
    TempoDataHandler tempoData;
    
    std::atomic<int> selectedDirection {2};
    
    ModulationMatrixProcessor modMatrix;
private:
    void DetermineIncrementsPerBlock(int numSamples); //free and sync
    void IncrementFree();
    void IncrementSync();
    void CheckForBounceCross();
    bool DetermineIsSyncAheadOfFree();
    void RestrictBounds();
    void SetModValue();
    
    void SetState(juce::ValueTree& lfoTree);
    void RemoveThisCurveAdjusterTreeFromAPVTS(juce::ValueTree& apvtsTree, juce::ValueTree& lfoTree);

    TempoSync::SyncTiming syncTiming;
    juce::String name {"LFO_Processor"};
    double sampleRate {44100.0};
    double incrementPerBlockFree{0.01};
    double incrementPerBlockSync{0.01};
    
    enum DirectionState
    {
        forwards, backwards
    };
    DirectionState freeDirection{forwards};
    DirectionState syncDirection{forwards};
    
    bool alreadyResetFree {false};
    bool shouldResetSyncFlag {false};
    
    //these are set to 1.0 so they don't trigger redundant reset of phases
    //the first call to IncrementSync() will have close to 0.0 values and then set these caches for useful comparison in future calls to IncrementSync()
    double cachedMeasurePositionFraction{1.0};
    double cachedQuarterNoteFraction{1.0};
    
    bool syncPhaseIsGreater;
    int cachedSyncIndex {-1};


    const juce::Identifier value_string_as_ID {"value"};
};
