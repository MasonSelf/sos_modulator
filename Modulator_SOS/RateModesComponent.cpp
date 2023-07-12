/*
  ==============================================================================

    RateModesComponent.cpp
    Created: 26 May 2023 2:16:14pm
    Author:  Mason Self

  ==============================================================================
*/

#include "RateModesComponent.h"

NonAutomatableArrowButton::NonAutomatableArrowButton(juce::String name, Direction d)
: juce::Button(name), direction(d)
{
    setToggleable(true);
    setClickingTogglesState(true);
}


void NonAutomatableArrowButton::paintButton (juce::Graphics& g, bool, bool)
{
    getToggleState() ?
        g.fillAll(juce::Colours::yellow) : g.fillAll(juce::Colours::grey);
    g.setColour(juce::Colours::black);
    
    switch (direction)
    {
        case forwards:
        {
            juce::Point<float> startOfArrow = {getWidth() / 5.0f, getHeight() / 2.0f};
            juce::Point<float> endOfArrow = {getWidth() / 5.0f * 4.0f, getHeight() / 2.0f};
            g.drawArrow({startOfArrow, endOfArrow}, lineThickness, arrowWidth, arrowHeight);
            break;
        }
        case backwards:
        {
            juce::Point<float> endOfArrow = {getWidth() / 5.0f, getHeight() / 2.0f};
            juce::Point<float> startOfArrow = {getWidth() / 5.0f * 4.0f, getHeight() / 2.0f};
            g.drawArrow({startOfArrow, endOfArrow}, lineThickness, arrowWidth, arrowHeight);
            break;
        }
        case bidirectional:
        {
            juce::Point<float> forwardsStartOfArrow = {getWidth() / 5.0f + arrowWidth, getHeight() / 2.0f};
            juce::Point<float> forwardsSndOfArrow = {getWidth() / 5.0f * 4.0f, getHeight() / 2.0f};
            g.drawArrow({forwardsStartOfArrow, forwardsSndOfArrow}, lineThickness, arrowWidth, arrowHeight);
            juce::Point<float> backwardsEndOfArrow = {getWidth() / 5.0f, getHeight() / 2.0f};
            juce::Point<float> backwardsStartOfArrow = {getWidth() / 5.0f * 4.0f - arrowWidth, getHeight() / 2.0f};
            g.drawArrow({backwardsStartOfArrow, backwardsEndOfArrow}, lineThickness, arrowWidth, arrowHeight);
            break;
        }
        case bounce:
        {
            {
                juce::Point<float> forwardsStartOfArrow = {getWidth() / 6.0f + arrowWidth, getHeight() / 2.0f};
                juce::Point<float> forwardsSndOfArrow = {getWidth() / 6.0f * 3.0f, getHeight() / 2.0f};
                g.drawArrow({forwardsStartOfArrow, forwardsSndOfArrow}, lineThickness, arrowWidth, arrowHeight);
                juce::Point<float> backwardsEndOfArrow = {getWidth() / 6.0f, getHeight() / 2.0f};
                juce::Point<float> backwardsStartOfArrow = {getWidth() / 6.0f * 3.0f - arrowWidth, getHeight() / 2.0f};
                g.drawArrow({backwardsStartOfArrow, backwardsEndOfArrow}, lineThickness, arrowWidth, arrowHeight);
            }
            {
                juce::Point<float> forwardsStartOfArrow = {getWidth() / 6.0f * 3.0f + arrowWidth, getHeight() / 2.0f};
                juce::Point<float> forwardsSndOfArrow = {getWidth() / 6.0f * 5.0f, getHeight() / 2.0f};
                g.drawArrow({forwardsStartOfArrow, forwardsSndOfArrow}, lineThickness, arrowWidth, arrowHeight);
                juce::Point<float> backwardsEndOfArrow = {getWidth() / 6.0f * 3.0f, getHeight() / 2.0f};
                juce::Point<float> backwardsStartOfArrow = {getWidth() / 6.0f * 5.0f - arrowWidth, getHeight() / 2.0f};
                g.drawArrow({backwardsStartOfArrow, backwardsEndOfArrow}, lineThickness, arrowWidth, arrowHeight);
            }
            break;
        }
    };

}



DirectionComponent::DirectionComponent(LFO_modProcessor& p, juce::String componentName)
: forwardsButton(componentName + " forwards", NonAutomatableArrowButton::Direction::forwards),
  backwardsButton(componentName + " backwards", NonAutomatableArrowButton::Direction::backwards),
  bidirectionalButton(componentName + " bidirectional", NonAutomatableArrowButton::Direction::bidirectional),
  bounceButton(componentName + " bounce", NonAutomatableArrowButton::Direction::bounce),
  processor(p)
{
    
    addAndMakeVisible(forwardsButton);
    addAndMakeVisible(backwardsButton);
    addAndMakeVisible(bidirectionalButton);
    addAndMakeVisible(bounceButton);
    
    forwardsButton.addListener(this);
    backwardsButton.addListener(this);
    bidirectionalButton.addListener(this);
    bounceButton.addListener(this);
    UpdateSelectionFromProcessor();
    startTimerHz(60);
}

DirectionComponent::~DirectionComponent()
{
    forwardsButton.removeListener(this);
    backwardsButton.removeListener(this);
    bidirectionalButton.removeListener(this);
    bounceButton.removeListener(this);
    if (isTimerRunning())
    {
        stopTimer();
    }
}

void DirectionComponent::timerCallback()
{
    if (processor.readyForGuiUpdate.load() == true)
    {
        UpdateSelectionFromProcessor();
        stopTimer();
    }
}

//ensures only one option is selected at a time
void DirectionComponent::buttonClicked (juce::Button* b)
{
    if (b->getName() == forwardsButton.getName())
    {

        if (b->getToggleState() == false)
        {
            b->setToggleState(true, juce::NotificationType::dontSendNotification);
        }
        else
        {
            backwardsButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            bidirectionalButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            bounceButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            processor.selectedDirection.store(0);
        }
    }
    if (b->getName() == backwardsButton.getName())
    {
        if (b->getToggleState() == false)
        {
            b->setToggleState(true, juce::NotificationType::dontSendNotification);
        }
        else
        {
            forwardsButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            bidirectionalButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            bounceButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            processor.selectedDirection.store(1);
        }
    }
    if (b->getName() == bidirectionalButton.getName())
    {
        if (b->getToggleState() == false)
        {
            b->setToggleState(true, juce::NotificationType::dontSendNotification);
        }
        else
        {
            forwardsButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            backwardsButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            bounceButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            processor.selectedDirection.store(2);
        }
    }
    if (b->getName() == bounceButton.getName())
    {
        if (b->getToggleState() == false)
        {
            b->setToggleState(true, juce::NotificationType::dontSendNotification);
        }
        else 
        {
            forwardsButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            backwardsButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            bidirectionalButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            processor.selectedDirection.store(3);
        }
    }
}

void DirectionComponent::resized()
{
    auto buttonWidth = getWidth() / 2;
    auto buttonHeight = getHeight() / 2;
    forwardsButton.setBounds(0, 0, buttonWidth, buttonHeight);
    backwardsButton.setBounds(buttonWidth, 0, buttonWidth, buttonHeight);
    bidirectionalButton.setBounds(0, buttonHeight, buttonWidth, buttonHeight);
    bounceButton.setBounds(buttonWidth, buttonHeight, buttonWidth, buttonHeight);
}

void DirectionComponent::UpdateSelectionFromProcessor()
{
    if (processor.selectedDirection.load() == 0)
    {
        forwardsButton.setToggleState(true, juce::NotificationType::sendNotification);
    }
    else if (processor.selectedDirection.load() == 1)
    {
        backwardsButton.setToggleState(true, juce::NotificationType::sendNotification);
    }
    else if (processor.selectedDirection.load() == 2)
    {
        bidirectionalButton.setToggleState(true, juce::NotificationType::sendNotification);
    }
    else
    {
        bounceButton.setToggleState(true, juce::NotificationType::sendNotification);
    }
}

