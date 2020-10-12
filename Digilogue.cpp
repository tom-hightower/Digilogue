#include "Digilogue.h"
#include "IPlug_include_in_plug_src.h"
#include "LFO.h"

Digilogue::Digilogue(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamMainGain)      ->  InitDouble("Gain", 100., 0., 100.0, 0.01, "%");
  GetParam(kParamMainTempo)     ->  InitDouble("Tempo", 120., 60., 220., 0.5, "bpm");
  GetParam(kParamMainOctave)    ->  InitInt("Octave", 4, 0, 7);
  GetParam(kParamNoteGlideTime) ->  InitMilliseconds("Note Glide Time", 0., 0.0, 30.);
  GetParam(kParamVCO1Octave)    ->  InitInt("VCO1 Octave", 4, 0, 7);
  GetParam(kParamVCO1Wave)      ->  InitEnum("VCO1 Wave", VCO<>::kTriangle, {VCO_SHAPE_VALIST});
  GetParam(kParamVCO1Pitch)     ->  InitInt("VCO1 Pitch", 0, -1200, 1200, "c");
  GetParam(kParamVCO1Shape)     ->  InitInt("VCO1 Shape", 0, 0, 1023);
  GetParam(kParamVCO2Octave)    ->  InitInt("VCO2 Octave", 4, 0, 7);
  GetParam(kParamVCO2Wave)      ->  InitEnum("VCO2 Wave", VCO<>::kTriangle, { VCO_SHAPE_VALIST });
  GetParam(kParamVCO2Pitch)     ->  InitInt("VCO2 Pitch", 0, -1200, 1200, "c");
  GetParam(kParamVCO2Shape)     ->  InitInt("VCO2 Shape", 0, 0, 1023);
  GetParam(kParamVCO2ModDepth)  ->  InitInt("Cross Mod Depth", 0, 0, 1023);
  GetParam(kParamVCO2PitchInt)  ->  InitInt("Pitch EG Int", 0, -4800, 4800);
  GetParam(kParamVCO2Sync)      ->  InitBool("Sync", false);
  GetParam(kParamVCO2Ring)      ->  InitBool("Ring", false);
  GetParam(kParamMixerVCO1)     ->  InitPercentage("VCO1");
  GetParam(kParamMixerVCO2)     ->  InitPercentage("VCO2");
  GetParam(kParamMixerNoise)    ->  InitPercentage("Noise");
  GetParam(kParamFilterCutoff)  ->  InitFrequency("Cutoff");
  GetParam(kParamFilterRes)     ->  InitInt("Resonance", 0, 0, 1023);
  GetParam(kParamFilterEGInt)   ->  InitPercentage("EG Int", 0., -100.);
  GetParam(kParamFilterPole)    ->  InitEnum("Filter Pole", LowPassFilter<>::k2Pole, {LPF_POLE_VALIST});
  GetParam(kParamFilterKeyTrack)->  InitDouble("Key Track", 0, 0, 100, 50, "%");
  GetParam(kParamFilterVelocity)->  InitDouble("Velocity", 0, 0, 100, 50, "%");
  GetParam(kParamAmpAttack)     ->  InitDouble("AmpAttack",  10., 1., 1000., 0.1, "ms", IParam::kFlagsNone, "AmpADSR", IParam::ShapePowCurve(3.));
  GetParam(kParamAmpDecay)      ->  InitDouble("AmpDecay",   10., 1., 1000., 0.1, "ms", IParam::kFlagsNone, "AmpADSR", IParam::ShapePowCurve(3.));
  GetParam(kParamAmpSustain)    ->  InitDouble("AmpSustain", 50., 0., 100.,    1, "%",  IParam::kFlagsNone, "AmpADSR");
  GetParam(kParamAmpRelease)    ->  InitDouble("AmpRelease", 10., 2., 1000., 0.1, "ms", IParam::kFlagsNone, "AmpADSR");
  GetParam(kParamAttack)        ->  InitDouble("Attack", 10., 1., 1000., 0.1, "ms", IParam::kFlagsNone, "ADSR", IParam::ShapePowCurve(3.));
  GetParam(kParamDecay)         ->  InitDouble("Decay", 10., 1., 1000., 0.1, "ms", IParam::kFlagsNone, "ADSR", IParam::ShapePowCurve(3.));
  GetParam(kParamSustain)       ->  InitDouble("Sustain", 50., 0., 100., 1, "%", IParam::kFlagsNone, "ADSR");
  GetParam(kParamRelease)       ->  InitDouble("Release", 10., 2., 1000., 0.1, "ms", IParam::kFlagsNone, "ADSR");
  GetParam(kParamLFOWave)       ->  InitEnum("LFO Shape", LFO<>::kTriangle, {LFO_SHAPE_VALIST});
  GetParam(kParamLFOMod)        ->  InitEnum("LFO Sync", DigilogueLFO<>::kOff, {LFO_MOD_VALIST});
  GetParam(kParamLFORateHz)     ->  InitFrequency("LFO Rate", 1., 0.01, 40.);
  GetParam(kParamLFORateTempo)  ->  InitEnum("LFO Rate", LFO<>::k1, {LFO_TEMPODIV_VALIST});
  GetParam(kParamLFORateMode)   ->  InitBool("LFO BPM Sync", false);
  GetParam(kParamLFOInt)        ->  InitPercentage("LFO Intensity");
  GetParam(kParamLFOTarget)     ->  InitEnum("LFO Target", DigilogueLFO<>::kShape, {LFO_TARGET_VALIST});
  GetParam(kParamDelayCutoff)   ->  InitFrequency("High Pass Cutoff");
  GetParam(kParamDelayTime)     ->  InitInt("Delay Time", 0, 0, 1023);
  GetParam(kParamDelayFeedback) ->  InitPercentage("Delay Feedback");
  GetParam(kParamDelayEnable)   ->  InitBool("Delay Enable", false);
  GetParam(kParamVoiceMode)     ->  InitEnum("Voice Mode", EVoiceModes::kPoly, {VOICE_MODE_VALIST});
    
#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_MID_GRAY);
    pGraphics->EnableMouseOver(true);
    pGraphics->EnableMultiTouch(true);
    
#ifdef OS_WEB
    pGraphics->AttachPopupMenuControl();
#endif

    pGraphics->EnableLiveEdit(true);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds().GetPadded(-20.f);
    const IRECT mainPanel = b.GetCentredInside(75.0f, 225.0f).GetHShifted(-460).GetVShifted(-150);
    pGraphics->AttachControl(new IVKnobControl(mainPanel.GetCentredInside(75).GetVShifted(-75), kParamMainGain, "Gain"));
    pGraphics->AttachControl(new IVKnobControl(mainPanel.GetCentredInside(75), kParamNoteGlideTime, "Glide"));
    const IRECT vcoPanel = b.GetCentredInside(300.0f, 2.0f / 3.0f * b.H()).GetHShifted(-385).GetVShifted(-64);
    const IRECT mixerPanel = b.GetCentredInside(75.0f, 2.0f / 3.0f * b.H()).GetHShifted(-85).GetVShifted(-64);
    const IRECT filterPanel = b.GetCentredInside(150.0f, 2.0f / 3.0f * b.H()).GetHShifted(-10).GetVShifted(-64);
    const IRECT egLFOPanel = b.GetCentredInside(300.0f, 2.0f / 3.0f * b.H()).GetHShifted(140).GetVShifted(64);
    const IRECT delayModePanel = b.GetCentredInside(225.0f, 2.0f / 3.0f * b.H()).GetHShifted(440).GetVShifted(-64);

    IRECT keyboardBounds = b.GetFromBottom(b.GetLengthOfShortestSide() / 3.0f);
    IRECT wheelsBounds = keyboardBounds.ReduceFromLeft(100.f).GetPadded(-10.f);
    pGraphics->AttachControl(new IVKeyboardControl(keyboardBounds), kCtrlTagKeyboard);
    pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5)), kCtrlTagBender);
    pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5, true), IMidiMsg::EControlChangeMsg::kModWheel));
//    pGraphics->AttachControl(new IVMultiSliderControl<4>(b.GetGridCell(0, 2, 2).GetPadded(-30), "", DEFAULT_STYLE, kParamAttack, EDirection::Vertical, 0.f, 1.f));
    const IRECT ampADSRPanel = egLFOPanel.GetGridCell(0, 0, 1, 4);
    pGraphics->AttachControl(new IVKnobControl(ampADSRPanel.GetGridCell(0, 1, 4).GetMidHPadded(20.), kParamAmpAttack, "Attack"),   kNoTag, "AmpADSR");
    pGraphics->AttachControl(new IVKnobControl(ampADSRPanel.GetGridCell(1, 1, 4).GetMidHPadded(20.), kParamAmpDecay,  "Decay"),    kNoTag, "AmpADSR");
    pGraphics->AttachControl(new IVKnobControl(ampADSRPanel.GetGridCell(2, 1, 4).GetMidHPadded(20.), kParamAmpSustain, "Sustain"), kNoTag, "AmpADSR");
    pGraphics->AttachControl(new IVKnobControl(ampADSRPanel.GetGridCell(3, 1, 4).GetMidHPadded(20.), kParamAmpRelease, "Release"), kNoTag, "AmpADSR");

    const IRECT adsrPanel = egLFOPanel.GetGridCell(0, 1, 1, 4);
    pGraphics->AttachControl(new IVKnobControl(adsrPanel.GetGridCell(0, 1, 4).GetMidHPadded(20.), kParamAttack, "Attack"), kNoTag, "ADSR");
    pGraphics->AttachControl(new IVKnobControl(adsrPanel.GetGridCell(1, 1, 4).GetMidHPadded(20.), kParamDecay, "Decay"), kNoTag, "ADSR");
    pGraphics->AttachControl(new IVKnobControl(adsrPanel.GetGridCell(2, 1, 4).GetMidHPadded(20.), kParamSustain, "Sustain"), kNoTag, "ADSR");
    pGraphics->AttachControl(new IVKnobControl(adsrPanel.GetGridCell(3, 1, 4).GetMidHPadded(20.), kParamRelease, "Release"), kNoTag, "ADSR");

    pGraphics->AttachControl(new IVLEDMeterControl<2>(vcoPanel.GetFromRight(100).GetPadded(-20)), kCtrlTagMeter);

    const IRECT lfoPanel = egLFOPanel.GetGridCell(0, 2, 1, 5);
    pGraphics->AttachControl(new IVKnobControl(lfoPanel.GetGridCell(0, 0, 2, 3).GetCentredInside(60), kParamLFORateHz, "Rate"), kNoTag, "LFO");
    pGraphics->AttachControl(new IVKnobControl(lfoPanel.GetGridCell(0, 1, 2, 3).GetCentredInside(60), kParamLFOInt, "Rate"), kNoTag, "LFO")->DisablePrompt(false);
    pGraphics->AttachControl(new IVKnobControl(lfoPanel.GetGridCell(0, 2, 2, 3).GetCentredInside(60), kParamLFOTarget, "Depth"), kNoTag, "LFO");
    pGraphics->AttachControl(new IVKnobControl(lfoPanel.GetGridCell(0, 3, 2, 3).GetCentredInside(60), kParamLFOWave, "Shape"), kNoTag, "LFO")->DisablePrompt(false);
    
    
    pGraphics->AttachControl(new IVButtonControl(keyboardBounds.GetFromTRHC(200, 30).GetTranslated(0, -30), SplashClickActionFunc,
      "Show/Hide Keyboard", DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE).WithLabelText({15.f, EVAlign::Middle})))->SetAnimationEndActionFunction(
      [pGraphics](IControl* pCaller) {
        static bool hide = false;
        pGraphics->GetControlWithTag(kCtrlTagKeyboard)->Hide(hide = !hide);
        pGraphics->Resize(PLUG_WIDTH, hide ? PLUG_HEIGHT / 2 : PLUG_HEIGHT, pGraphics->GetDrawScale());
    });
#ifdef OS_IOS
    if(!IsAuv3AppExtension())
    {
      pGraphics->AttachControl(new IVButtonControl(b.GetFromTRHC(100, 100), [pGraphics](IControl* pCaller) {
                               dynamic_cast<IGraphicsIOS*>(pGraphics)->LaunchBluetoothMidiDialog(pCaller->GetRECT().L, pCaller->GetRECT().MH());
                               SplashClickActionFunc(pCaller);
                             }, "BTMIDI"));
    }
#endif
    
    pGraphics->SetQwertyMidiKeyHandlerFunc([pGraphics](const IMidiMsg& msg) {
                                              dynamic_cast<IVKeyboardControl*>(pGraphics->GetControlWithTag(kCtrlTagKeyboard))->SetNoteFromMidi(msg.NoteNumber(), msg.StatusMsg() == IMidiMsg::kNoteOn);
                                           });
  };
#endif
}

#if IPLUG_DSP
void Digilogue::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  mDSP.ProcessBlock(nullptr, outputs, 2, nFrames, mTimeInfo.mPPQPos, mTimeInfo.mTransportIsRunning);
  mMeterSender.ProcessBlock(outputs, nFrames, kCtrlTagMeter);
  mLFOVisSender.PushData({kCtrlTagLFOVis, {float(mDSP.mLFO.GetLastOutput())}});
}

void Digilogue::OnIdle()
{
  mMeterSender.TransmitData(*this);
  mLFOVisSender.TransmitData(*this);
}

void Digilogue::OnReset()
{
  mDSP.Reset(GetSampleRate(), GetBlockSize());
}

void Digilogue::ProcessMidiMsg(const IMidiMsg& msg)
{
  TRACE;
  
  int status = msg.StatusMsg();
  
  switch (status)
  {
    case IMidiMsg::kNoteOn:
    case IMidiMsg::kNoteOff:
    case IMidiMsg::kPolyAftertouch:
    case IMidiMsg::kControlChange:
    case IMidiMsg::kProgramChange:
    case IMidiMsg::kChannelAftertouch:
    case IMidiMsg::kPitchWheel:
    {
      goto handle;
    }
    default:
      return;
  }
  
handle:
  mDSP.ProcessMidiMsg(msg);
  SendMidiMsg(msg);
}

void Digilogue::OnParamChange(int paramIdx)
{
  mDSP.SetParam(paramIdx, GetParam(paramIdx)->Value());
}

bool Digilogue::OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData)
{
  if(ctrlTag == kCtrlTagBender && msgTag == IWheelControl::kMessageTagSetPitchBendRange)
  {
    const int bendRange = *static_cast<const int*>(pData);
    mDSP.mSynth.SetPitchBendRange(bendRange);
  }
  
  return false;
}
#endif
