/*
  ==============================================================================

    SyncTiming.h
    Created: 21 May 2023 11:03:44pm
    Author:  Mason Self

  ==============================================================================
*/

#pragma once
#include <juce_core/juce_core.h>

namespace TempoSync
{

enum SyncOptions
{
    eightbars, fourbars, threebars, twobars, bar, halfnote, quarternote, eighthnote, sixteenthnote, thirtysecond
};

const juce::StringArray SyncStrings
{
    "8 bars", "4 bars", "3 bars", "2 bars", "bar", "half note", "quarter note", "eighth note", "sixteenth note", "thirtysecond note"
};

class SyncTiming
{
public:
    void UpdateSampleRate(double sR)
    {
        sampleRate = sR;
    }

    
    double GetIncrementPerSample(int syncChoiceIndex, float bpm, int timeSigNum, int timeSigDenom)
    {
        auto sync = GetSyncOption(syncChoiceIndex);
        
        double beatsPerSec = static_cast<double>(bpm) / 60.0;
        double samplesPerQuarterNote = sampleRate / beatsPerSec;
        if (sync == SyncOptions::thirtysecond)
        {
            return 1.0 / (samplesPerQuarterNote / 8.0);
        }
        if (sync == SyncOptions::sixteenthnote)
        {
            return 1.0 / (samplesPerQuarterNote / 4.0);
        }
        else if (sync == SyncOptions::eighthnote)
        {
            return 1.0 / (samplesPerQuarterNote / 2.0);
        }
        else if (sync == SyncOptions::quarternote)
        {
            return 1.0 / samplesPerQuarterNote;
        }
        else if (sync == SyncOptions::halfnote)
        {
            return 1.0 / (samplesPerQuarterNote * 2.0);
        }
        else
        {
            //calculate samples per bar
            auto timeSigDenomMult = 4.0 / static_cast<double>(timeSigDenom);
            auto samplesPerBar = samplesPerQuarterNote * timeSigNum * timeSigDenomMult;
            
            if (sync == SyncOptions::bar)
            {
                return 1.0 / samplesPerBar;
            }
            else if (sync == SyncOptions::twobars)
            {
                return 1.0 / (samplesPerBar * 2.0);
            }
            else if (sync == SyncOptions::threebars)
            {
                return 1.0 / (samplesPerBar * 3.0);
            }
            else if (sync == SyncOptions::fourbars)
            {
                return 1.0 / (samplesPerBar * 4.0);
            }
            else if (sync == SyncOptions::eightbars)
            {
                return 1.0 / (samplesPerBar * 8.0);
            }
        }
        jassertfalse; //should have found something!!
        return 0.0;
    }
    
    SyncOptions GetSyncOption(int index)
    {
        switch (index)
        {
            case 0: return SyncOptions::eightbars;
            case 1: return SyncOptions::fourbars;
            case 2: return SyncOptions::threebars;
            case 3: return SyncOptions::twobars;
            case 4: return SyncOptions::bar;
            case 5: return SyncOptions::halfnote;
            case 6: return SyncOptions::quarternote;
            case 7: return SyncOptions::eighthnote;
            case 8: return SyncOptions::sixteenthnote;
            case 9: return SyncOptions::thirtysecond;
        }
        jassertfalse; //should have found a match
        return SyncOptions::bar;
    }
    
private:
    double sampleRate {44100.0};
};
}
