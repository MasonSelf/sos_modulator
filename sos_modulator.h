#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

      ID:               sos_modulator
      vendor:           Mason Self
      version:          0.0.1
      name:             sos_modulator
      description:      ModulatorSynthsOfSelf
      license:          GPL/Commercial
      dependencies:     juce_core, juce_audio_utils,juce_gui_basics, juce_graphics, juce_audio_processors,sos_curve_adjuster, sos_sliders, sos_IAudioProcessor, sos_popup_window

     END_JUCE_MODULE_DECLARATION

#endif

#include "Modulator_SOS/CenteredLabel.h"
#include "Modulator_SOS/DestinationAttenuverter.h"
#include "Modulator_SOS/DestinationMatrixComponent.h"
#include "Modulator_SOS/DirectionModes.h"
#include "Modulator_SOS/LFO_modEditor.h"
#include "Modulator_SOS/LFO_modProcessor.h"
#include "Modulator_SOS/ModulationMatrixData.h"
#include "Modulator_SOS/ModulationMatrixProcessor.h"
#include "Modulator_SOS/ModulationPopUpWindow.h"
#include "Modulator_SOS/ModulatorCurveAdjusterTrace.h"
#include "Modulator_SOS/RateModesComponent.h"
#include "Modulator_SOS/SOSToggleButton.h"
#include "Modulator_SOS/SyncTiming.h"
#include "Modulator_SOS/TempoDataHandler.h"

