/*
  ==============================================================================

    ModulationPopUpWindow.cpp
    Created: 28 May 2023 5:25:38pm
    Author:  Mason Self

  ==============================================================================
*/

#include "ModulationPopUpWindow.h"

ModulationPopUpWindow::ModulationPopUpWindow(IAudioProcessor& p, LFO_modProcessor& _lfoModProcessor,
                      float curveAdjusterWidth, float curveAdjusterHeight,
                      int resetParamIndex,
                      int freezeParamIndex,
                      int freeRateParamIndex,
                      int syncRateParamIndex,
                      int rateBlendParamIndex,
                      int ampParamIndex,
                      std::vector<juce::String> modulationSourceNames,
                      std::vector<juce::String> modulationDestinationNames,
                     juce::Colour backgroundColor,
                     juce::Colour textColor,
                     juce::Colour syncRateColor,
                     juce::Colour freeRateColor,
                     juce::Colour matrixPositiveColor = juce::Colours::lightgreen,
                     juce::Colour matrixNegativeColor = juce::Colours::red)
: SOSPopUpWindowManager("Modulation", "parameter mod", 15, backgroundColor, textColor),
  modEditor(p,
            _lfoModProcessor,
            curveAdjusterWidth,
            curveAdjusterHeight,
            resetParamIndex,
            freezeParamIndex,
            freeRateParamIndex,
            syncRateParamIndex,
            rateBlendParamIndex,
            ampParamIndex,
            std::move(modulationSourceNames),
            std::move(modulationDestinationNames),
            textColor,
            freeRateColor,
            syncRateColor,
            matrixPositiveColor,
            matrixNegativeColor)
{
    setInterceptsMouseClicks(false, true); //no clicks on this, but children are ok
    shouldDisplayWindow.addListener(this);
    addAndMakeVisible(tab);
}

ModulationPopUpWindow::~ModulationPopUpWindow()
{
    shouldDisplayWindow.removeListener(this);
}
void ModulationPopUpWindow::resized()
{
    popUpWindow.setBounds(0, 0, getWidth(), getHeight());
    
    const auto tabHeight = 60;
    const auto tabWidth = 80;
    tab.setBounds(getWidth() - tabWidth, 0, tabWidth, tabHeight);
    
    modEditor.setBounds(60, 15, getWidth() - 120, getHeight() );
}
void ModulationPopUpWindow::valueChanged (juce::Value&)
{
    if (shouldDisplayWindow.getValue() == juce::var(true))
    {
        removeChildComponent(&tab);
        addAndMakeVisible(popUpWindow);
        addAndMakeVisible(modEditor);
    }
    else
    {
        removeChildComponent(&popUpWindow);
        removeChildComponent(&modEditor);
        addAndMakeVisible(tab);
    }
}
