/*
  ==============================================================================

    LFO_Processor.cpp
    Created: 21 May 2023 10:03:51pm
    Author:  Mason Self

  ==============================================================================
*/

#include "LFO_modProcessor.h"

LFO_modProcessor::LFO_modProcessor(std::string n, juce::AudioProcessorValueTreeState& _apvts,
                                   const juce::Identifier& rateFreeParamName,
                                   const juce::Identifier& rateSyncParamName,
                                   const juce::Identifier& rateBlendParamName,
                                   const juce::Identifier& resetParamName,
                                   const juce::Identifier& freezeParamName,
                                   const juce::Identifier& scaleParamName,
                                   std::vector<juce::String> modDestinations)
: curveAdjusterProcessor(n, 0.0f, 0.02),
rateFreeParamID(rateFreeParamName),
rateSyncParamID(rateSyncParamName),
rateBlendParamID(rateBlendParamName),
resetParamID(resetParamName),
freezeParamID(freezeParamName),
scaleParamID(scaleParamName),
apvts(_apvts),
modMatrix(modDestinations, {"mod output"})
{
    name += juce::String(" ") + n;
}

void LFO_modProcessor::Init(double _sampleRate)
{
    sampleRate = _sampleRate;
    syncTiming.UpdateSampleRate(sampleRate);
}
void LFO_modProcessor::Process(int numSamples)
{
    DetermineIncrementsPerBlock(numSamples);
    
    syncPhaseIsGreater = DetermineIsSyncAheadOfFree(); //store prior state for comparison in CheckForBounceCross()
    IncrementFree();
    IncrementSync();
    CheckForBounceCross();
    RestrictBounds();
    SetModValue();
    
}

void LFO_modProcessor::DetermineIncrementsPerBlock(int numSamples)
{
    //free
    incrementPerBlockFree = (*apvts.getRawParameterValue(rateFreeParamID)) / sampleRate * static_cast<double>(numSamples);
    while (incrementPerBlockFree > 1.0)
    {
        incrementPerBlockFree -= 1.0;
    }
    
    //sync
    incrementPerBlockSync = syncTiming.GetIncrementPerSample(dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(rateSyncParamID))->getIndex(), tempoData.tempo.load(), tempoData.timeSigNum.load(), tempoData.timeSigDenom.load()) * static_cast<double>(numSamples);
    while (incrementPerBlockSync > 1.0)
    {
        incrementPerBlockSync -= 1.0;
    }
    
    if (selectedDirection.load() == 0) //forwards
    {
        if (freeDirection != forwards)      //free
        {
            freeDirection = forwards;
        }
        if (syncDirection != forwards)      //sync
        {
            syncDirection = forwards;
        }
    }
    else if (selectedDirection.load() == 1) //backwards
    {
        if (freeDirection != backwards)         //free
        {
            freeDirection = backwards;
        }
        if (syncDirection != backwards)         //sync
        {
            syncDirection = backwards;
        }
    }
}
                                                              
void LFO_modProcessor::IncrementFree()
{
    if (dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(resetParamID))->get()
        && shouldResetFreeFlag == false
        && ! dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(freezeParamID))->get())
    {
        shouldResetFreeFlag = true;
        freePhase.store(0.0);
        return;
    }
    else if (! dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(resetParamID))->get() && shouldResetFreeFlag == true)
    {
        shouldResetFreeFlag = false;
    }

    if (! dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(freezeParamID))->get())
    {
        if (freeDirection == forwards)
        {
            freePhase.store(freePhase.load() + incrementPerBlockFree);
            
            if (freePhase.load() > 1.0 && selectedDirection.load() == 0) //forwards
            {
                freePhase.store(freePhase.load() - 1.0);
            }
            else if (freePhase.load() >= 1.0)
            {
                if (selectedDirection.load() == 2 || selectedDirection.load() == 3) //bidirectional or bounce
                {
                    freeDirection = backwards;
                }
            }
        }
        if (freeDirection == backwards)
        {
            freePhase.store(freePhase.load() - incrementPerBlockFree);
            
            if (freePhase.load() < 0.0 && selectedDirection.load() == 1) // backwards
            {
                freePhase.store(freePhase.load() + 1.0);
            }
            else if (freePhase.load() <= 0.0)
            {
                if (selectedDirection.load() == 2 || selectedDirection.load() == 3) //bidirectional or bounce
                {
                    freeDirection = forwards;
                    freePhase.store(freePhase.load() + incrementPerBlockFree);
                }
            }
        }
    }
}

void LFO_modProcessor::IncrementSync()
{
    //check if reset button is on
    if (dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(resetParamID))->get() &&
        shouldResetSyncFlag == false)
    {
        shouldResetSyncFlag = true;
        syncWasReset = false;
    }

    //only increment if freeze is NOT on
    if (! dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(freezeParamID))->get())
    {
//        //tighten loop for small subdivisions
//        if (selectedDirection != 3 /*bounce*/ &&
//            tempoData.quarterNoteWithinBar < 0.01f)
//        {
//            auto selectedIndex = static_cast<int>(*apvts.getRawParameterValue(rateSyncParamID));
//            if (selectedIndex == TempoSync::SyncOptions::quarternote || selectedIndex == TempoSync::SyncOptions::eighthnote || selectedIndex == TempoSync::SyncOptions::sixteenthnote
//                || selectedIndex == TempoSync::SyncOptions::thirtysecond)
//            {
//                shouldResetSyncFlag = true;
//                syncWasReset = false;
//            }
//        }
            
        if (syncDirection == forwards)
        {
            syncPhase.store(syncPhase.load() + incrementPerBlockSync);
            
            if (syncPhase.load() > 1.0 && selectedDirection == 0) //forwards
            {
                syncPhase.store(syncPhase.load() - 1.0);
            }
            else if (syncPhase.load() >= 1.0)
            {
                if (selectedDirection == 2 || selectedDirection == 3) //bidirectional or bounce
                {
                    syncDirection = backwards;
                    if (syncPhase.load() > 1.0)
                    {
                        auto reflectedValue = 1.0 - (syncPhase.load() - 1.0); //only bounce off on 1.0 the value it passed 1.0
                        syncPhase.store(reflectedValue);
                    }
                }
            }
        }
        else if (syncDirection == backwards)
        {
            syncPhase.store(syncPhase.load() - incrementPerBlockSync);
            
            if (syncPhase.load() < 0.0 && selectedDirection == 1) //backwards
            {
                syncPhase.store(syncPhase.load() + 1.0);
            }
            else if (syncPhase.load() <= 0.0)
            {
                if (selectedDirection == 2 || selectedDirection == 3) //bidirectional or bounce
                {
                    syncDirection = forwards;
                    
                    if (syncPhase.load() < 0.0)
                    {
                        auto reflectedValue = syncPhase.load() * -1.0; //only bounce off of 0.0 as much as it crossed into negative
                        syncPhase.store(reflectedValue);
                    }
                }
            }
        }
    }
    

    auto selectedIndex = static_cast<int>(*apvts.getRawParameterValue(rateSyncParamID));
    if (tempoData.justStartedPlaying.load() == true || selectedIndex != cachedSyncIndex)
    {
        shouldResetSyncFlag = true;
        syncWasReset = false;
        tempoData.justStartedPlaying.store(false);
    }
    
    cachedSyncIndex = selectedIndex;
    
    
    if (shouldResetSyncFlag && ! syncWasReset && ! dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(freezeParamID))->get())
    {
        //6/8 support needed! 
        auto syncChoice = syncTiming.GetSyncOption(static_cast<int>(*apvts.getRawParameterValue(rateSyncParamID)));
        if (syncChoice == TempoSync::SyncOptions::eightbars ||
            syncChoice == TempoSync::SyncOptions::fourbars ||
            syncChoice == TempoSync::SyncOptions::threebars ||
            syncChoice == TempoSync::SyncOptions::twobars ||
            syncChoice == TempoSync::SyncOptions::bar)
        {
            if (tempoData.quarterNoteWithinBar < 0.01f) //first beat of measure
            {
                syncPhase.store(0.0);
                syncWasReset = true;
                shouldResetSyncFlag = false;
            }
        }
        else 
        {
            if ((tempoData.quarterNoteWithinBar - static_cast<float>(static_cast<int>(tempoData.quarterNoteWithinBar))) < 0.01f)
            {
                syncPhase.store(0.0);
                syncWasReset = true;
                shouldResetSyncFlag = false;
            }
        }
    }
}

void LFO_modProcessor::CheckForBounceCross()
{
    if (selectedDirection == 3) //bounce
    {
        if (syncPhaseIsGreater != DetermineIsSyncAheadOfFree())
        {
            //free
            if (freePhase == forwards)
            {
                freeDirection = backwards;
                freePhase.store(freePhase.load() - incrementPerBlockFree);
            }
            else
            {
                freeDirection = forwards;
                freePhase.store(freePhase.load() + incrementPerBlockFree);
            }
            //sync
            if (syncDirection == forwards)
            {
                syncDirection = backwards;
                syncPhase.store(syncPhase.load() - incrementPerBlockSync);
            }
            else
            {
                syncDirection = forwards;
                syncPhase.store(syncPhase.load() + incrementPerBlockSync);
            }
        }
    }
}

bool LFO_modProcessor::DetermineIsSyncAheadOfFree()
{
    return syncPhase > freePhase;
}

void LFO_modProcessor::RestrictBounds()
{
    if (syncPhase < 0.0)
    {
        syncPhase = 0.0;
    }
    else if (syncPhase > 1.0)
    {
        syncPhase = 1.0;
    }
    
    if (freePhase < 0.0)
    {
        freePhase = 0.0;
    }
    else if (freePhase > 1.0)
    {
        freePhase = 1.0;
    }
}

void LFO_modProcessor::SetModValue()
{
    float blendVal = *apvts.getRawParameterValue(rateBlendParamID);
    float invertedBlendVal = 1.0f - blendVal;

    auto blendedPhase = freePhase.load() * invertedBlendVal + syncPhase.load() * blendVal;
    value.store(curveAdjusterProcessor.GetTranslatedOutput(blendedPhase) * (*apvts.getRawParameterValue(scaleParamID)));
}

void LFO_modProcessor::SaveState(juce::AudioProcessorValueTreeState& stateToAppendTo)
{
    //use this to clear the points if needed when debugging
    //return;
    
    juce::ValueTree temp
    { name, {},
        {
            { "direction" , {{value_string_as_ID, selectedDirection.load()}}}
        }
    };
    
    if (stateToAppendTo.state.getChildWithName(name).isValid())
    {
        stateToAppendTo.state.getChildWithName(name).copyPropertiesAndChildrenFrom(temp, nullptr);
    }
    else
    {
        stateToAppendTo.state.appendChild(temp, nullptr);
    }
    
    curveAdjusterProcessor.SaveState(stateToAppendTo);
    modMatrix.SaveState(stateToAppendTo);
}
void LFO_modProcessor::LoadAndRemoveStateFromAPTVS(juce::ValueTree& apvtsTree)
{
    //dbug usage
    //return;
    auto lfoTree = apvtsTree.getChildWithName(name);
    if (lfoTree.isValid())
    {
        SetState(lfoTree);
        RemoveThisCurveAdjusterTreeFromAPVTS(apvtsTree, lfoTree);
        
        curveAdjusterProcessor.LoadAndRemoveStateFromAPTVS(apvtsTree);
        modMatrix.LoadAndRemoveStateFromAPTVS(apvtsTree);
    }
}

void LFO_modProcessor::SetState(juce::ValueTree& lfoTree)
{
    auto directionChild = lfoTree.getChildWithName("direction");
    if (!directionChild.isValid())
    {
        return;
    }


    selectedDirection.store((int)directionChild.getProperty(value_string_as_ID, -1.0));
    if (selectedDirection.load() == -1)
    {
        selectedDirection.store(2);
    }
    readyForGuiUpdate.store(true);
}
void LFO_modProcessor::RemoveThisCurveAdjusterTreeFromAPVTS(juce::ValueTree& apvtsTree, juce::ValueTree& lfoTree)
{
    apvtsTree.removeChild(lfoTree, nullptr);
}

float LFO_modProcessor::GetModdedVal(const juce::Identifier& paramID, const juce::String& destinationName)
{
    auto modAsPercentage = modMatrix.GetModulationValue(destinationName) * value.load();
    auto paramMax = apvts.getParameterRange(paramID).end;
    auto paramMin = apvts.getParameterRange(paramID).start;

    auto moddedVal = *apvts.getRawParameterValue(paramID) + modAsPercentage * ( paramMax - paramMin);
    if (moddedVal < paramMin)
    {
        moddedVal = paramMin;
    }
    else if (moddedVal > paramMax)
    {
        moddedVal = paramMax;
    }
    return moddedVal;
}
