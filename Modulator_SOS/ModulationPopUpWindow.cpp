/*
  ==============================================================================

    ModulationPopUpWindow.cpp
    Created: 28 May 2023 5:25:38pm
    Author:  Mason Self

  ==============================================================================
*/

#include "ModulationPopUpWindow.h"

ModulationPopUpWindow::ModulationPopUpWindow(AudioPluginAudioProcessor& p, LFO_modProcessor& _lfoModProcessor,
                      float curveWidth, float curveHeight,
                      int resetParamIndex,
                      int freezeParamIndex,
                      int freeRateParamIndex,
                      int syncRateParamIndex,
                      int rateBlendParamIndex,
                      int ampParamIndex,
                      std::vector<juce::String> modulationSourceNames, std::vector<juce::String> modulationDestinationNames)
: SOSPopUpWindowManager("Modulation", "parameter mod", 15, WHITE),
  modEditor(p, _lfoModProcessor, 150.0f, 150.0f, 1, 2, 3, 4, 5, 6, modulationSourceNames, modulationDestinationNames, juce::Colours::black, RED, BLUE)
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
