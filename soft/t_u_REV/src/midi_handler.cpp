// Copyright (C)2021 - Eduard Heidt
//
// Author: Eduard Heidt (eh2k@gmx.de)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//

#include "extern/stmlib_midi.h"
#include <stdlib.h>
#include <Arduino.h>
#include "TU_digital_inputs.h"

struct MidiHandlerImpl
{
  bool playing = false;
  uint32_t bpm = 0;

  static MidiHandlerImpl *instance()
  {
    static MidiHandlerImpl _instance;
    return &_instance;
  }

  static void RawByte(uint8_t byte)
  {
  }

  static void BozoByte(uint8_t byte)
  {
  }

  static void Aftertouch(uint8_t byte, uint8_t byte2, uint8_t byte3 = 0)
  {
  }

  static void SysExEnd()
  {
  }

  static void SysExStart()
  {
  }

  static void SysExByte(uint8_t byte)
  {
  }

  static uint8_t CheckChannel(uint8_t channel)
  {
    return false;
  }

  static inline void NoteOn(uint8_t channel, uint8_t key, uint8_t velocity)
  {
  }

  static inline void NoteOff(uint8_t channel, uint8_t key, uint8_t velocity)
  {
  }

  static inline void PitchBend(uint8_t channel, uint16_t pitch_bend)
  {
  }

  static inline void ControlChange(uint8_t channel, uint8_t controller, uint8_t value)
  {
  }

  static void ProgramChange(uint8_t channel, uint8_t program)
  {
  }

  static void Clock()
  {
#define ONE_POLE(out, in, coefficient) out += (coefficient) * ((in)-out);

    static int cnt = 0;
    if (++cnt > 24)
    {
      cnt = 0;
      static uint32_t last_clock = 0;
      uint32_t ticks = micros() - last_clock;
      //uint32_t bpm = (60 * 1000 * 1000 * 25) / (ticks / 24);
      uint32_t bpm = (600 * 1000.0f * 1000.0f) / ticks;

      if (abs(bpm - instance()->bpm) > 1)
        instance()->bpm = bpm;

      ONE_POLE(instance()->bpm, bpm, 0.1f);

      last_clock = micros();
    }

    TU::DigitalInputs::clock<TU::DigitalInput::DIGITAL_INPUT_1>();
  }

  static void Start()
  {
    instance()->playing = true;
  }

  static void Continue()
  {
  }

  static void Stop()
  {
    instance()->playing = false;
    Reset();
  }

  static void ActiveSensing()
  {
  }

  static void RawMidiData(uint8_t status, uint8_t *data, uint8_t data_size, uint8_t accepted_channel)
  {
  }

  static void Reset()
  {
  }
};

namespace TU
{
  void midiReceive(uint8_t midiByte)
  {
    static stmlib_midi::MidiStreamParser<MidiHandlerImpl> midi_stream;
    midi_stream.PushByte(midiByte);
  }

  uint32_t midiBPM()
  {
    if (MidiHandlerImpl::instance()->playing)
      return MidiHandlerImpl::instance()->bpm;
    else
      return 0;
  }

  bool midiPlaying()
  {
    return MidiHandlerImpl::instance()->playing;
  }
}