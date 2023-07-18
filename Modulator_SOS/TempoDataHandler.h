/*
  ==============================================================================

    TempoDataHandler.h
    Created: 28 Mar 2023 6:26:04am
    Author:  thegr

  ==============================================================================
*/

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

struct TempoDataHandler
{
    std::atomic<double> tempo{ 0 };
    std::atomic<int> timeSigNum{ 4 };
    std::atomic<int> timeSigDenom{ 4 };
    std::atomic<double> sampleRate{ 44100.0 };
    std::atomic<bool> justStartedPlaying{false};
    std::atomic<double> quarterNoteWithinBar;

    void ProcessTempoData(juce::Optional<juce::AudioPlayHead::PositionInfo> info)
    {
        if (info->getPpqPosition().hasValue() && info->getPpqPositionOfLastBarStart().hasValue())
        {
            quarterNoteWithinBar.store(*(info->getPpqPosition()) - *(info->getPpqPositionOfLastBarStart()));
        }

        bool isPlaying = info->getIsPlaying();
        HandleJustStartedPlayingFlag(isPlaying);
        auto possibleBpm = info->getBpm();
        if (possibleBpm.hasValue())
        {
            tempo = *possibleBpm;
        }

        auto possibleTimeSig = info->getTimeSignature();
        if (possibleTimeSig.hasValue())
        {
            timeSigNum = possibleTimeSig->numerator;
            timeSigDenom = possibleTimeSig->denominator;
        }
    }

private:
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

    bool previousPlayingState {false};
};
