/*
  ==============================================================================

    DestinationAttenuverter.h
    Created: 27 May 2023 1:05:14am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class DestinationAttenuverter : public juce::Component, public juce::Slider::Listener
{
public:
    DestinationAttenuverter(juce::String _source, juce::String _destination, float value, juce::Slider& slider);
    ~DestinationAttenuverter() override;
    void paint(juce::Graphics& g) override;
    void mouseDoubleClick(const juce::MouseEvent&) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void sliderValueChanged(juce::Slider*) override;


    juce::Value localValue {0.0f}; 
    
    const juce::String source, destination;
private:
    juce::String GetValueAsString();
    juce::Colour GetAttenuatedColor(juce::Colour polarizationColor);
    juce::Point<float> startDragPos{0.0f, 0.0f};
    juce::Slider& attenuationSlider;
};
