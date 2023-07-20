/*
  ==============================================================================

    DestinationMatrix.cpp
    Created: 27 May 2023 1:04:55am
    Author:  Mason Self

  ==============================================================================
*/

#include "DestinationMatrixComponent.h"

DestinationMatrixComponent::DestinationMatrixComponent(std::vector<juce::String> destinations,
                                                       std::vector<juce::String> sources,
                                                       ModulationMatrixProcessor& p,
                                                       juce::Colour _textColor,
                                                       juce::Slider& _attenuationSlider)
: processor(p), textColor(_textColor), attenuationSlider(_attenuationSlider)
{
    setInterceptsMouseClicks(false, true);
    
    //add source labels (only once)
    for (auto& s : sources)
    {
        sourceNames.emplace_back(std::make_unique<CenteredLabel>(s,s, textColor));
        addAndMakeVisible(sourceNames.back().get());
    }
    
    int i = 0;
    for (auto& d : destinations)
    {
        //add to desintation labels
        destinationNames.push_back(std::make_unique<CenteredLabel>(d, d, textColor));
        addAndMakeVisible(destinationNames.back().get());
        for (auto& s : sources)
        {
            //add attenuverter component
            attenuverterComponents.emplace_back(std::make_unique<DestinationAttenuverter>(s, d, processor.modVals[i].get(), attenuationSlider));
            
            addAndMakeVisible(attenuverterComponents.back().get());
            
            //register listener to new component
            attenuverterComponents.at(i)->localValue.addListener(this);
            
            ++i;
        }
    }
    startTimerHz(60);
}

DestinationMatrixComponent::~DestinationMatrixComponent()
{
    stopTimer();
}

void DestinationMatrixComponent::resized()
{
    int destinationColumnWidth = 50;
    int attenBoxWidth = (getWidth() - destinationColumnWidth) / sourceNames.size();
    int attenBoxHeight = getHeight() / (destinationNames.size() + 1);
    
    //destination labels
    for (int i = 0; i < destinationNames.size(); ++i)
    {
        destinationNames.at(i)->setBounds(0, attenBoxHeight * i + attenBoxHeight, destinationColumnWidth, attenBoxHeight);
    }
    
    //source labels
    for (int i = 0; i < sourceNames.size(); ++i)
    {
        sourceNames.at(i)->setBounds(destinationColumnWidth + attenBoxWidth * i, 0, attenBoxWidth, attenBoxHeight);
    }
    
    //attenuverter components
    for (int i = 0; i < attenuverterComponents.size(); ++i)
    {
        int x = destinationColumnWidth + (i % sourceNames.size()) * attenBoxWidth;
        int y = attenBoxHeight + (i / sourceNames.size()) * attenBoxHeight;
        attenuverterComponents.at(i)->setBounds(x, y, attenBoxWidth, attenBoxHeight);
    }
    
}

void DestinationMatrixComponent::timerCallback()
{
    if (processor.readyForGuiInit.get())
    {
        processor.readyForGuiInit.set(false);
        for (int i = 0; i < attenuverterComponents.size(); ++i)
        {
            attenuverterComponents.at(i)->localValue.setValue(processor.modVals[i].value.load());
            attenuverterComponents.at(i)->repaint();
        }
        stopTimer();
    }
}

void DestinationMatrixComponent::valueChanged(juce::Value& v)
{
    for (int i = 0; i < attenuverterComponents.size(); ++i)
    {
        if (v.refersToSameSourceAs(attenuverterComponents.at(i)->localValue))
        {
            processor.modVals[i].value.store(static_cast<float>(v.getValue()));
        }
    }
}
