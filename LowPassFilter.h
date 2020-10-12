#pragma once

#define LPF_POLE_VALIST "2-Pole", "4-Pole"

template<typename T = double>
class LowPassFilter
{
public:
  enum EPole {
    k2Pole,
    k4Pole,
    kNumPoles
  };
};