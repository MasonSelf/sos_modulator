/*
  ==============================================================================

    ModulatorCurveAdjusterTrace.h
    Created: 23 May 2023 3:21:24am
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <sos_curve_adjuster/sos_curve_adjuster.h>


struct ModulatorCurveAdjusterTrace : public MouseIgnoringComponent
{
    float xLeft{0.0f};
    float yTop{0.0f};
    float yBottom{0.0f};
    
    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::grey);
        g.setOpacity(0.8f);
        g.drawVerticalLine(static_cast<int>(xLeft), yTop, yBottom);
        g.drawHorizontalLine(static_cast<int>(yTop), xLeft, getRight());
    }
};
