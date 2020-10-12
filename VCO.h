#pragma once

/**
 * @file
 * @brief Basic VCO implementation
 * @author Tom Hightower
 */

#include "Oscillator.h"

#define VCO_SHAPE_VALIST "Triangle", "Square", "Sawtooth"


class VCO : public IOscillator<>
{
public:
  enum EShape {
    kTriangle,
    kSquare,
    kSawtooth,
    kNumShapes
  };

  VCO(double startPhase = 0., double startFreq = 1., EShape wave = EShape::kTriangle, int octave = 4, int shape = 0, int pitch = 0)
  : IOscillator<T>(startPhase, startFreq)
  {

  }
};