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
    eightBars, fourBars, threeBars, twoBars, oneBar, halfNote, quarter, quarterTriplet, eighth, eighthTriplet, sixteenth, sixteenthQuintuplet, sixteenthTriplet, thirtysecond, thirtysecondTriplet, sixtyfourth
};

const juce::StringArray SyncStrings
{
    "8 bars", "4 bars", "3 bars", "2 bars", "bar", "1/2", "1/4", "1/4T", "1/8", "1/8T", "1/16", "1/16Q", "1/16T", "1/32", "1/32T", "1/64"
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
        if (sync == SyncOptions::sixtyfourth)
        {
            return 1.0 / (samplesPerQuarterNote / 16.0);
        }
        else if (sync == SyncOptions::thirtysecondTriplet)
        {
            return 1.0 / (samplesPerQuarterNote / 12.0);
        }
        else if (sync == SyncOptions::thirtysecond)
        {
            return 1.0 / (samplesPerQuarterNote / 8.0);
        }
        else if (sync == SyncOptions::sixteenthTriplet)
        {
            return 1.0 / (samplesPerQuarterNote / 6.0);
        }
        else if (sync == SyncOptions::sixteenthQuintuplet)
        {
            return 1.0 / (samplesPerQuarterNote / 5.0);
        }
        else if (sync == SyncOptions::sixteenth)
        {
            return 1.0 / (samplesPerQuarterNote / 4.0);
        }
        else if (sync == SyncOptions::eighthTriplet)
        {
            return 1.0 / (samplesPerQuarterNote / 3.0);
        }
        else if (sync == SyncOptions::eighth)
        {
            return 1.0 / (samplesPerQuarterNote / 2.0);
        }
        else if (sync == SyncOptions::quarterTriplet)
        {
            return 1.0 / (samplesPerQuarterNote * 2.0 / 3.0);
        }
        else if (sync == SyncOptions::quarter)
        {
            return 1.0 / samplesPerQuarterNote;
        }
        else if (sync == SyncOptions::halfNote)
        {
            return 1.0 / (samplesPerQuarterNote * 2.0);
        }
        else
        {
            //calculate samples per bar
            auto timeSigDenomMult = 4.0 / static_cast<double>(timeSigDenom);
            auto samplesPerBar = samplesPerQuarterNote * timeSigNum * timeSigDenomMult;
            
            if (sync == SyncOptions::oneBar)
            {
                return 1.0 / samplesPerBar;
            }
            else if (sync == SyncOptions::twoBars)
            {
                return 1.0 / (samplesPerBar * 2.0);
            }
            else if (sync == SyncOptions::threeBars)
            {
                return 1.0 / (samplesPerBar * 3.0);
            }
            else if (sync == SyncOptions::fourBars)
            {
                return 1.0 / (samplesPerBar * 4.0);
            }
            else if (sync == SyncOptions::eightBars)
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
            case 0:  return SyncOptions::eightBars;
            case 1:  return SyncOptions::fourBars;
            case 2:  return SyncOptions::threeBars;
            case 3:  return SyncOptions::twoBars;
            case 4:  return SyncOptions::oneBar;
            case 5:  return SyncOptions::halfNote;
            case 6:  return SyncOptions::quarter;
            case 7:  return SyncOptions::quarterTriplet;
            case 8:  return SyncOptions::eighth;
            case 9:  return SyncOptions::eighthTriplet;
            case 10: return SyncOptions::sixteenth;
            case 11: return SyncOptions::sixteenthQuintuplet;
            case 12: return SyncOptions::sixteenthTriplet;
            case 13: return SyncOptions::thirtysecond;
            case 14: return SyncOptions::thirtysecondTriplet;
            case 15: return SyncOptions::sixtyfourth;
        }
        jassertfalse; //should have found a match
        return SyncOptions::oneBar;
    }
    
private:
    double sampleRate {44100.0};
};
}
