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

/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../JuceLibraryCode/BinaryData.h"

#include "Push2Demo.h"
#include "Macros.h"

#include <sstream>
#include <iomanip>
#include <deque>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {
      setSize (960, 600);

      // specify the number of input and output channels that we want to open
      setAudioChannels (2, 2);

      // Initialize a label to show the status of the connection
      status_.setColour(Label::textColourId, Colours::white);
      status_.setJustificationType(Justification::bottomLeft);
      addAndMakeVisible(status_);

      // Starts the demo class animating the display
      NBase::Result result = demo_.Init();
      if (result.Succeeded())
      {
        status_.setText("Push 2 connected", juce::dontSendNotification);
        demo_.SetMidiInputCallback(
                                   [this](const MidiMessage& message)
                                   {
                                     this->processMidiInput(message);
                                   });
      }
      else
      {
        status_.setText(result.GetDescription(), juce::dontSendNotification);
      }
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    void processMidiInput(const MidiMessage& message)
    {
      if (message.getRawDataSize() == 3)
      {
        auto data = message.getRawData();
        std::ostringstream oss;
        oss << "Midi ("
          << message.getTimeStamp() << ") :"
          << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << int(data[0]) << " - "
          << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << int(data[1]) << " - "
          << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << int(data[2]);

        midiMessageStack_.push_back(oss.str());
        if (midiMessageStack_.size() > 4)
        {
          midiMessageStack_.pop_front();
        }

        const MessageManagerLock lock;

        std::string status;
        for (auto s : midiMessageStack_)
        {
          status += s + "\n";
        }
        status_.setText(status, juce::dontSendNotification);
      }
    }

    //=======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()

        MUnused(samplesPerBlockExpected);
        MUnused(sampleRate);

    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //=======================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);

        auto logo = ImageCache::getFromMemory(BinaryData::PushStartup_png, BinaryData::PushStartup_pngSize);
        g.drawImageAt(logo, (getWidth() - logo.getWidth()) / 2 , (getHeight() - logo.getHeight()) / 2);
    }

    void resized() override
    {
      const int kStatusSize = 100;
      status_.setBounds(0, getHeight() - kStatusSize, getWidth(), kStatusSize);
    }


private:
    //==============================================================================

    // Your private member variables go here...

    Demo demo_;
    Label status_;
    std::deque<std::string> midiMessageStack_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
