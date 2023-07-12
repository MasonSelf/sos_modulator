/*
  ==============================================================================

    DestinationMatrixProcessor.cpp
    Created: 28 May 2023 9:35:18am
    Author:  Mason Self

  ==============================================================================
*/

#include "ModulationMatrixProcessor.h"

ModulationMatrixProcessor::ModulationMatrixProcessor(std::vector<juce::String> _destinationNames, std::vector<juce::String> _sourceNames)
: destinationNames(_destinationNames), sourceNames(_sourceNames)
{
    modVals.numVals.set(static_cast<int>(destinationNames.size() * sourceNames.size()));
}

float ModulationMatrixProcessor::GetModulationValue(const juce::String& destinationName)
{
    for (int i = 0; i < destinationNames.size(); ++i)
    {
        if (destinationNames.at(i) == destinationName)
        {
            //get all values in column
            int rowStartIndex = i * static_cast<int>(sourceNames.size());
            int rowLastIndex = rowStartIndex + static_cast<int>(sourceNames.size()) - 1;
            float sumMod {0.0f};
            while (rowStartIndex <= rowLastIndex)
            {
                sumMod += ConvertModulationValueAsPercentage(rowStartIndex);
                ++rowStartIndex;
            }
            return sumMod;
        }
    }
    jassertfalse; //invalid destination name!
    return 0.0f;
}

void ModulationMatrixProcessor::SaveState(juce::AudioProcessorValueTreeState& stateToAppendTo)
{
    //use this to clear the points if needed when debugging
    //return;
    
    juce::ValueTree temp
    { name, {},
        {
            { modValCollectionChildName , {}}
        }
    };
    
    for (int i = 0; i < modVals.numVals.get(); ++i)
    {
        juce::Identifier modValName = juce::String("mod" + std::to_string(i));
        juce::ValueTree modValTree
        { modValName, {},
            {
                {modValChildName, {{value_string_as_ID, modVals[i].get()}}}
            }
        };
        temp.getChildWithName(modValCollectionChildName).appendChild(modValTree, nullptr);
    }
    
    
    if (stateToAppendTo.state.getChildWithName(name).isValid())
    {
        stateToAppendTo.state.getChildWithName(name).copyPropertiesAndChildrenFrom(temp, nullptr);
    }
    else
    {
        stateToAppendTo.state.appendChild(temp, nullptr);
    }
}

void ModulationMatrixProcessor::LoadAndRemoveStateFromAPTVS(juce::ValueTree& apvtsTree)
{
    //dbug usage
    //return;
    
    auto matrixTree = apvtsTree.getChildWithName(name);
    if (matrixTree.isValid())
    {
        SetState(matrixTree);
        RemoveThisCurveAdjusterTreeFromAPVTS(apvtsTree, matrixTree);
    }
}

float ModulationMatrixProcessor::ConvertModulationValueAsPercentage(int index)
{
    return modVals[index].value.load() / 100.0f;
}


void ModulationMatrixProcessor::SetState(juce::ValueTree& lfoTree)
{
    auto setOfmodValsChild = lfoTree.getChildWithName(modValCollectionChildName);
    if (!setOfmodValsChild.isValid())
    {
        return;
    }

    for (int i = 0; i < setOfmodValsChild.getNumChildren(); ++i)
    {
        auto connectorChild = setOfmodValsChild.getChild(i);
        modVals[i].set((float)connectorChild.getChildWithName(modValChildName).getProperty(value_string_as_ID, 0.0));
    }
    
    readyForGuiInit.set(true);
}

void ModulationMatrixProcessor::RemoveThisCurveAdjusterTreeFromAPVTS(juce::ValueTree& apvtsTree, juce::ValueTree& matrixTree)
{
    apvtsTree.removeChild(matrixTree, nullptr);
}
