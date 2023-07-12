/*
  ==============================================================================

    DestinationAttenuverter.cpp
    Created: 27 May 2023 1:05:14am
    Author:  Mason Self

  ==============================================================================
*/

#include "DestinationAttenuverter.h"

DestinationAttenuverter::DestinationAttenuverter(juce::String _source, juce::String _destination, float value)
: localValue(value), source(_source), destination(_destination)
{
}

void DestinationAttenuverter::paint(juce::Graphics& g)
{
    if (localValue > 0.0f)
    {
        g.fillAll(juce::Colours::lightgreen);
    }
    else if (localValue < 0.0f)
    {
        g.fillAll(juce::Colours::red);
    }
    else //0.0f
    {
        g.fillAll(juce::Colours::white);
    }
    g.setColour(juce::Colours::black);
    g.setFont(15.0f);
    g.drawFittedText(GetValueAsString(), 0, 0, getWidth(), getHeight(), juce::Justification::centred, 1);
}

void DestinationAttenuverter::mouseDoubleClick(const juce::MouseEvent&)
{
    localValue = 0.0f;
    repaint();
}

void DestinationAttenuverter::mouseDown(const juce::MouseEvent& e)
{
    startDragPos = e.position;
}

void DestinationAttenuverter::mouseDrag(const juce::MouseEvent& e)
{
    localValue = static_cast<float>(localValue.getValue()) - (e.position.y - startDragPos.y) / 2.0f;
    startDragPos = e.position;
    if (localValue > 100.0f)
    {
        localValue = 100.0f;
    }
    else if (localValue < -100.0f)
    {
        localValue = -100.0f;
    }
    repaint();
}

juce::String DestinationAttenuverter::GetValueAsString()
{
    juce::String result(localValue.toString());
    if (localValue > 0.0f)
    {
        result = "+" + result;
    }
    result = result.substring(0, 5);
    return result + "%";
}
