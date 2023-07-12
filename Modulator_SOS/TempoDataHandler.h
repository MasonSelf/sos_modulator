/*
  ==============================================================================

    TempoDataHandler.h
    Created: 28 Mar 2023 6:26:04am
    Author:  thegr

  ==============================================================================
*/

#pragma once
#include <juce_core/juce_core.h>

struct TempoDataHandler
{
    std::atomic<double> tempo{ 0 };
    std::atomic<int> timeSigNum{ 4 };
    std::atomic<int> timeSigDenom{ 4 };
    std::atomic<double> sampleRate{ 44100.0 };
    std::atomic<bool> justStartedPlaying{false};
    std::atomic<double> quarterNoteWithinBar;
    
    void HandleJustStartedPlayingFlag(bool isPlaying)
    {
        if (isPlaying && previousPlayingState == false)
        {
            justStartedPlaying.store(true);
        }
        else if (! isPlaying )
        {
            justStartedPlaying.store(false);
        }
        previousPlayingState = isPlaying;
    }
    
private:
    bool previousPlayingState {false};
};
