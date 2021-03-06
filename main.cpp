#include "miosix.h"
#include "../drivers/common/audio.h"
#include "audio/audio_processor.h"
#include "drivers/double_encoder.h"
#include "drivers/midi_in_port.h"
#include "audio_processors/mono_synth.h"
#include "audio_modules/envelope.h"
#include "audio_modules/lowpass_filter_1p.h"
#include "audio_modules/lowpass_filter_ladder_4p.h"
#include "midi/midi.h"
#include "tests/midi_test_data.h"
#include "tests/hardware_tests/encoder_test.h"
#include "tests/hardware_tests/double_encoder_test.h"
#include "tests/hardware_tests/button_test.h"
#include "drivers/hardware_configuration.h"
#include "synth_configuration.h"
#include <array>
#include <queue>
#include <cmath>
#include <thread>

///////// TESTING //////////
///////////////////////////
/**
 * Enable this flag and a single specific test
 * to disable the main execution and start testing.
 */
//#define TESTING_HARDWARE

/**
 * Enable one of these flags to run a specific test.
 */
//#define TEST_BUTTONS
//#define TEST_ENCODERS
#define TEST_DOUBLE_ENCODERS
///////////////////////////
///////////////////////////

#ifndef TESTING_HARDWARE

// The audio driver
AudioDriver audioDriver;

// The synth engine
MonoSynth monoSynth(audioDriver);

// double encoders
DoubleEncoder doubleEncoder1(DoubleEncoder1Timer, DoubleEncoder1PortEnc,
                             DoubleEncoder1PinAEnc, DoubleEncoder1PinBEnc,
                             DoubleEncoder1PortBut, DoubleEncoder1PinBut); // TODO: verify why this encoder doesn't work
DoubleEncoder doubleEncoder2(DoubleEncoder2Timer, DoubleEncoder2PortEnc,
                             DoubleEncoder2PinAEnc, DoubleEncoder2PinBEnc,
                             DoubleEncoder2PortBut, DoubleEncoder2PinBut);
DoubleEncoder doubleEncoder3(DoubleEncoder3Timer, DoubleEncoder3PortEnc,
                             DoubleEncoder3PinAEnc, DoubleEncoder3PinBEnc,
                             DoubleEncoder3PortBut, DoubleEncoder3PinBut);
DoubleEncoder doubleEncoder4(DoubleEncoder4Timer, DoubleEncoder4PortEnc,
                             DoubleEncoder4PinAEnc, DoubleEncoder4PinBEnc,
                             DoubleEncoder4PortBut, DoubleEncoder4PinBut);
DoubleEncoder doubleEncoder5(DoubleEncoder5Timer, DoubleEncoder5PortEnc,
                             DoubleEncoder5PinAEnc, DoubleEncoder5PinBEnc,
                             DoubleEncoder5PortBut, DoubleEncoder5PinBut);

/**
 * Thread function for handling the hardware inputs.
 */
void hardwareInterfaceThreadFunc() {
    // encoders
    std::array < DoubleEncoder * , 5 > encoders = {&doubleEncoder1, &doubleEncoder2,
                                                   &doubleEncoder3, &doubleEncoder4, &doubleEncoder5};
    for (auto enc : encoders) enc->setSensitivity(EncoderSensitivity);
    encoders[1]->setValue(0.0f); // glideTime
//    encoders[1]->setPressedValue(0.5f);
    encoders[2]->setValue(1.0f); // lowpassCutoff
    encoders[2]->setPressedValue(0.0f); // filter cutoff envelope amount
    encoders[3]->setValue(0.0f); // lowpassResonance
    encoders[4]->setValue(0.5f); // envelope releaseTime
    encoders[4]->setPressedValue(0.0f); // envelope attackTime

    // synth parameters
    volatile float glideTime;
    volatile float detune;
    volatile float attackTime;
    volatile float releaseTime;
    volatile float lowpassCutoff;
    volatile float filterCutoffModAmt;
    volatile float lowpassResonance;

    // encoder loop
    while (true) {

        {
            miosix::FastMutex mutex;

            // synth audio modules
            Envelope &ampEnvelope = monoSynth.getEnvelope();
            LowpassFilterLadder4p &lowpassFilter = monoSynth.getLowpassFilter();

            for (auto enc : encoders) enc->update(); // updating the encoder state

            // getting values from the encoders
//            noteChangeTime = AudioMath::linearMap(encoders[1]->getPressedValue(), 0.0f, 1.0f, 600.0f, 50.0f);
            glideTime = encoders[1]->getValue() + 0.0001f;
            lowpassCutoff = encoders[2]->getValue();
            filterCutoffModAmt = encoders[2]->getPressedValue();
            lowpassResonance = encoders[3]->getValue();
            releaseTime = encoders[4]->getValue();
            attackTime = encoders[4]->getPressedValue();

            // setting the synth
            monoSynth.setGlide(glideTime);
            ampEnvelope.setAttack(attackTime);
            ampEnvelope.setRelease(releaseTime);
            monoSynth.setFilterCutoff(lowpassCutoff);
            monoSynth.setFilterCutoffEnvelopeAmt(filterCutoffModAmt);
            monoSynth.setFilterResonance(lowpassResonance);
        }
        miosix::Thread::sleep(30);

    }
}

void synthThreadFunc() {
    Midi::MidiParser midiParser; // midi parser
    MidiInPort &midiInPort = MidiInPort::getInstance();
    auto &midiBuffer = midiParser.getMessageQueue();

    while (true) {
        midiInPort.fillTheQueue(1); // thread yields here
        midiParser.parse(midiInPort.getRawMidiQueue());
        while (!midiBuffer.empty()) {
            Midi::MidiMessage tmp = midiBuffer.front();
            midiBuffer.pop();
            {
                miosix::FastMutex mutex;
                monoSynth.processMidi(tmp);
            }
        }
    }
}

int main() {
    // initializing the audio driver
    audioDriver.init();
    audioDriver.setAudioProcessable(monoSynth);

    // midi configuration

    // synth setup
    Envelope &ampEnvelope = monoSynth.getEnvelope();
    LowpassFilterLadder4p &lowpassFilter = monoSynth.getLowpassFilter();
    monoSynth.setGlide(0.005f);
    ampEnvelope.setSustain(1.0f);
    ampEnvelope.setDecay(0.2f);

    // starting the threads
    std::thread hardwareInterfaceThread(hardwareInterfaceThreadFunc);
    std::thread synthThread(synthThreadFunc);

    // starting the audio driver (blocking function)
    audioDriver.start();

    // join with the thread just for debugging
//    hardwareInterfaceThread.join();
//    synthThread.join();
//    midiThread.join();
}

#else // testing enabled
#ifdef TEST_BUTTONS
int main() {
    buttonTest();
}
#endif // TEST_BUTTONS

#ifdef TEST_ENCODERS
int main() {
    encoderTest();
}
#endif // TEST_ENCODERS

#ifdef TEST_DOUBLE_ENCODERS
int main() {
    doubleEncoderTest();
}
#endif

#endif // TESTING_HARDWARE
