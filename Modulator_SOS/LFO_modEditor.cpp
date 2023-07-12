/*
  ==============================================================================

    CurveAdjusterLFO.cpp
    Created: 21 May 2023 9:52:45pm
    Author:  Mason Self

  ==============================================================================
*/

#include "LFO_modEditor.h"

LFO_modEditor::LFO_modEditor(IAudioProcessor& p, LFO_modProcessor& _lfoModProcessor,
              float curveWidth, float curveHeight,
              int resetParamIndex,
              int freezeParamIndex,
              int freeRateParamIndex,
              int syncRateParamIndex,
              int rateBlendParamIndex,
              int ampParamIndex,
              std::vector<juce::String> modulationSourceNames, std::vector<juce::String> modulationDestinationNames,
             juce::Colour _textColor,
             juce::Colour _freeColor,
             juce::Colour _syncColor)
: lfoModProcessor(_lfoModProcessor),
  resetButton(p, lfoModProcessor.resetParamID, resetParamIndex, "reset"),
  freezeButton(p, lfoModProcessor.freezeParamID, freezeParamIndex, "freeze"),
  curve(curveWidth, curveHeight, lfoModProcessor.curveAdjusterProcessor, true, true, false),
  directionMode(_lfoModProcessor, "direction_mode"),
  freeRate(p, lfoModProcessor.rateFreeParamID, freeRateParamIndex, _freeColor, _freeColor, _textColor,  " hz"),
  syncRate(p, lfoModProcessor.rateSyncParamID, syncRateParamIndex, _syncColor, _syncColor, _textColor),
  rateBlend(p, lfoModProcessor.rateBlendParamID, rateBlendParamIndex, juce::Colours::grey, juce::Colours::black, juce::Colours::black),
  ampSlider(p, lfoModProcessor.scaleParamID, ampParamIndex, juce::Colours::grey, juce::Colours::black, juce::Colours::black),
  destinationMatrix(std::move(modulationDestinationNames), std::move(modulationSourceNames), lfoModProcessor.modMatrix, juce::Colours::black),
  textColor(_textColor),
  freeColor(_freeColor),
  syncColor(_syncColor)
{
    addAndMakeVisible(resetButton);
    addAndMakeVisible(freezeButton);
    addAndMakeVisible(curve);
    addAndMakeVisible(trace);
    addAndMakeVisible(directionMode);
    addAndMakeVisible(freeRate);
    addAndMakeVisible(syncRate);
    addAndMakeVisible(rateBlend);
    addAndMakeVisible(ampSlider);
    addAndMakeVisible(destinationMatrix);
    startTimerHz(60);
}

LFO_modEditor::~LFO_modEditor()
{
    stopTimer();
}

void LFO_modEditor::paint(juce::Graphics& g)
{
    float rateTracerY = curve.getBottom() + 10.0f;
    float rateTracerDiameter = 10.0f;
    float tracerCenterY = rateTracerY + rateTracerDiameter / 2.0f;
    auto freeX = lfoModProcessor.freePhase.load() * curve.getWidth() + curve.getX();
    auto syncX = lfoModProcessor.syncPhase.load() * curve.getWidth() + curve.getX();
    g.setColour(juce::Colours::grey);
    g.setOpacity(0.8f);
    if (freeX < syncX)
    {
        g.drawHorizontalLine(tracerCenterY, freeX, syncX);
    }
    else
    {
        g.drawHorizontalLine(tracerCenterY, syncX, freeX);
    }
    g.setColour(freeColor);
    g.setOpacity(1.0f);
    g.fillEllipse(freeX - rateTracerDiameter / 2.0f, rateTracerY, rateTracerDiameter, rateTracerDiameter);
    g.setColour(syncColor);
    g.setOpacity(1.0f);
    g.fillEllipse(syncX - rateTracerDiameter / 2.0f, rateTracerY, rateTracerDiameter, rateTracerDiameter);
    
    //draw trace on curveAdjuster
    trace.yBottom = tracerCenterY - trace.getY();
    
    auto blendAmount = lfoModProcessor.apvts.getRawParameterValue(lfoModProcessor.rateBlendParamID)->load();
    trace.xLeft = freeX * (1.0f - blendAmount) + syncX * blendAmount - trace.getX();
    trace.yTop = curve.GetY_AtX(trace.xLeft);

    trace.repaint();
    
    //label text
    
    g.setFont(15.0f);
    g.setColour(textColor);
    g.drawFittedText("direction", directionMode.getX(), 0, directionMode.getWidth(), directionMode.getY(), juce::Justification::centred, 1);
    g.drawFittedText("rates", freeRate.getX(), freeRate.getY() - 15, syncRate.getRight() - freeRate.getX(), 15, juce::Justification::centred, 1);
    g.drawFittedText("rate blend", freeRate.getX(), rateBlend.getBottom() - 12, syncRate.getRight() - freeRate.getX(), 15, juce::Justification::centred, 1);
    g.drawFittedText("attenuate", ampSlider.getX() - 20, ampSlider.getY() - 25, ampSlider.getWidth() + 40, 15, juce::Justification::centred, 1);
    g.setFont(9.0f);
    g.drawFittedText(GetFreeRatePercentageString(), freeRate.getX() + 22, freeRate.getBottom() + 10, 25, 10, juce::Justification::centred, 1);
    g.drawFittedText(GetSyncRatePercentageString(), syncRate.getX() + 22, syncRate.getBottom() + 10, 25, 10, juce::Justification::centred, 1);
    //draw scaled output
//    g.setColour(juce::Colours::yellow);
//    g.fillEllipse(curve.getRight() - 2, ((1.0f - lfoModProcessor.value.load()) * curve.GetHeight() + curve.getY()) - 2, 4, 4);
}

void LFO_modEditor::timerCallback()
{
    repaint();
}
void LFO_modEditor::resized()
{
    int lrMargin = 4;
    int verticalMargin = 4;
    int componentHeight = getHeight() - verticalMargin * 2;
    float segmentWidth = (getWidth() - lrMargin * 2) / 7.0f;
    
    int currentX = lrMargin + lrMargin;
    
    
    curve.setBounds(currentX, verticalMargin , curve.GetWidth(), curve.GetHeight());
    trace.setBounds(curve.getBounds().withBottom(curve.getBottom() + 50));
    
    resetButton.setBounds(currentX + lrMargin, curve.getBottom() + 24, curve.GetWidth() / 2 - lrMargin, 20);
    freezeButton.setBounds(resetButton.getRight() + lrMargin, curve.getBottom() + 24, curve.GetWidth() / 2 - lrMargin, 20);
    
    currentX = curve.getRight() + lrMargin * 4;
    directionMode.setBounds(currentX + 15, verticalMargin + 26, segmentWidth * 2 - 30, componentHeight / 8);
    //auto rateSectionY = verticalMargin + 140;
    freeRate.setBounds(currentX, directionMode.getBottom() + verticalMargin + 25, segmentWidth , componentHeight / 4);
    syncRate.setBounds(currentX + freeRate.getWidth(), directionMode.getBottom() + verticalMargin + 25, segmentWidth, componentHeight / 4);
    
    rateBlend.setBounds(currentX, freeRate.getBottom() + verticalMargin, segmentWidth * 2, componentHeight / 4);
    
    currentX = rateBlend.getRight() + lrMargin * 8;
    ampSlider.setBounds(currentX, curve.getX() + 23, 30, curve.GetHeight() + 18);
    
    destinationMatrix.setBounds(ampSlider.getRight() + lrMargin * 8, ampSlider.getY() + 20, 110, 110);
}

juce::String LFO_modEditor::GetFreeRatePercentageString()
{
    auto normalizedVal = 1.0f - static_cast<float>(rateBlend.getValue());
    juce::String result;
    return result.toDecimalStringWithSignificantFigures(normalizedVal * 100.0f, 3) + "%";
}

juce::String LFO_modEditor::GetSyncRatePercentageString()
{
    auto normalizedVal = static_cast<float>(rateBlend.getValue());
    juce::String result;
    return result.toDecimalStringWithSignificantFigures(normalizedVal * 100.0f, 3) + "%";
}
