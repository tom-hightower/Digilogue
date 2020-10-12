#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"
#include "Digilogue_DSP.h"

#define VOICE_MODE_VALIST "Poly", "Duo", "Unison", "Mono", "Chord", "Delay", "Arp", "Sidechain"

const int kNumPresets = 1;

enum EParams
{
  kParamMainGain = 0,
  kParamMainTempo,
  kParamMainOctave,
  kParamNoteGlideTime,
  kParamVCO1Octave,
  kParamVCO1Wave,
  kParamVCO1Pitch,
  kParamVCO1Shape,
  kParamVCO2Octave,
  kParamVCO2Wave,
  kParamVCO2Pitch,
  kParamVCO2Shape,
  kParamVCO2ModDepth,
  kParamVCO2PitchInt,
  kParamVCO2Sync,
  kParamVCO2Ring,
  kParamMixerVCO1,
  kParamMixerVCO2,
  kParamMixerNoise,
  kParamFilterCutoff,
  kParamFilterRes,
  kParamFilterEGInt,
  kParamFilterPole,
  kParamFilterKeyTrack,
  kParamFilterVelocity,
  kParamAmpAttack,
  kParamAmpDecay,
  kParamAmpSustain,
  kParamAmpRelease,
  kParamAttack,
  kParamDecay,
  kParamSustain,
  kParamRelease,
  kParamLFOWave,
  kParamLFOMod,
  kParamLFORateHz,
  kParamLFORateTempo,
  kParamLFORateMode,
  kParamLFOInt,
  kParamLFOTarget,
  kParamDelayCutoff,
  kParamDelayTime,
  kParamDelayFeedback,
  kParamDelayEnable,
  kParamVoiceMode,
  kNumParams
};

enum EControlTags
{
  kCtrlTagMeter = 0,
  kCtrlTagLFOVis,
  kCtrlTagScope,
  kCtrlTagRTText,
  kCtrlTagKeyboard,
  kCtrlTagBender,
  kNumCtrlTags
};

enum EVoiceModes
{
  kPoly,
  kDuo,
  kUnison,
  kMono,
  kChord,
  kDelay,
  kArp,
  kSidechain,
  kNumModes
};

using namespace iplug;
using namespace igraphics;

class Digilogue final : public Plugin
{
public:
  Digilogue(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
public:
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void OnIdle() override;
  bool OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData) override;

private:
  DigilogueDSP<sample> mDSP {16};
  IPeakSender<2> mMeterSender;
  ISender<1> mLFOVisSender;
#endif
};
