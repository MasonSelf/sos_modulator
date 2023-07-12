/*
  ==============================================================================

    ModulationMatrixData.h
    Created: 28 May 2023 4:30:24pm
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <juce_core/juce_core.h>
struct ModulationMatrixData
{
    juce::Atomic<float> mod0{0.0f};
    juce::Atomic<float> mod1{0.0f};
    juce::Atomic<float> mod2{0.0f};
    juce::Atomic<float> mod3{0.0f};
    juce::Atomic<float> mod4{0.0f};
    juce::Atomic<float> mod5{0.0f};
    juce::Atomic<float> mod6{0.0f};
    juce::Atomic<float> mod7{0.0f};
    juce::Atomic<float> mod8{0.0f};
    //add more as needed
    
    const int maxNumVals {9};
    
    juce::Atomic<int> numVals{0};

    juce::Atomic<float>& operator[](int index)
    {
        jassert(index < (maxNumVals - 1) && index < numVals.value.load()); //index is out of range!
        
        switch(index)
        {
            case 0:
                return mod0;
            case 1:
                return mod1;
            case 2:
                return mod2;
            case 3:
                return mod3;
            case 4:
                return mod4;
            case 5:
                return mod5;
            case 6:
                return mod6;
            case 7:
                return mod7;
            default:
                return mod8;
        }
    }
};
