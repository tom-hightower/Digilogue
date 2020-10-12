#pragma once

#include "LFO.h"

#define LFO_MOD_VALIST "Off", "Rate", "Intensity"
#define LFO_TARGET_VALIST "Cutoff", "Pitch", "Shape"


template<typename T = double>
class DigilogueLFO : public LFO<T>
{
public:
  enum EEGMod
  {
    kOff,
    kRate,
    kInt,
    kNumMods
  };

  enum ETarget
  {
    kCutoff,
    kPitch,
    kShape,
    kNumTargets
  };
};