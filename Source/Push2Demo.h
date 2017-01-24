// Copyright (c) 2017 Ableton AG, Berlin
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#pragma once

#include "push2/JuceToPush2DisplayBridge.h"
#include <assert.h>

/*!
 *  A simple demo class that will draw some animation on the push display
 *  and listen to incoming midi input from push
 */

class Demo
  : public Timer
  , public MidiInputCallback
{
public:

  /*!
   *  Initialises and starts the demo
   *
   *  \return the result of the initialisation process
   */

  NBase::Result Init();


  using midicb_t = std::function<void(const MidiMessage&)>;

  /*!
   *  Allows a client to hook a collback in order to process midi messages recieved from push2
   */

  void SetMidiInputCallback(const midicb_t& func);

private:

  /*!
   *  renders a frame and send it to the push display
   */

  void drawFrame();

  /*!
   *  look for the push 2 input device and starts listening to it
   *
   *  \return the result of the initialisation process
   */

  NBase::Result openMidiDevice();

  /*!
   *  the juce midi incoming message callback
   *  @see juce::MidiInputCallback
   */

  void handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message) override;

  /*!
   *  the juce timer callback
   *  @see juce::Timer
   */

  void timerCallback() override;


private:
  ableton::Push2DisplayBridge bridge_;    /*!< The bridge allowing to use juce::graphics for push */
  ableton::Push2Display push2Display_;                  /*!< The low-level push2 class */
  std::unique_ptr<MidiInput> midiInput_;  /*!< Push2's midi input */
  midicb_t midiCallback_;                 /*!> The midi callback to call when incoming messages are recieved */
  float elapsed_;                         /*!> Fake elapsed time used for the animation */
};