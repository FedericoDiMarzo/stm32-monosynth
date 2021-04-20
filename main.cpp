#include "miosix.h"
#include "../drivers/common/audio.h"
#include "audio/audio_processor.h"
#include "drivers/double_encoder.h"
#include "audio_processors/mono_synth.h"
#include "audio_modules/envelope.h"
#include "audio_modules/lowpass_filter_1p.h"
#include "midi/midi.h"
#include "tests/midi_test_data.h"
#include "tests/hardware_tests/encoder_test.h"
#include "tests/hardware_tests/double_encoder_test.h"
#include "tests/hardware_tests/button_test.h"
#include "drivers/hardware_configuration.h"
#include <array>
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
AudioDriver audioDriver(SampleRate::_44100Hz);

// The synth engine
MonoSynth monoSynth(audioDriver);

// to change the speed of the sequencer
volatile float noteChangeTime;

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

std::vector <Midi::MidiMessage> midiMessagesFromNoteValues(uint8_t *noteArray, size_t size) {
    std::vector <Midi::MidiMessage> midiQueue;
    for (size_t i = 0; i < size; i++) {
        midiQueue.push_back(Midi::MidiMessage({noteArray[i], 100}, Midi::MidiMessageType::NOTE_ON));
        midiQueue.push_back(Midi::MidiMessage({noteArray[i], 100}, Midi::MidiMessageType::NOTE_OFF));
    }
    return midiQueue;
}

/**
 * Thread function for handling the hardware inputs.
 */
void hardwareInterfaceThreadFunc() {
    // encoders
    std::array < DoubleEncoder * , 5 > encoders =
                                           {&doubleEncoder1, &doubleEncoder2,
                                            &doubleEncoder3, &doubleEncoder4, &doubleEncoder5};
    for (auto enc : encoders) enc->setSensitivity(EncoderSensitivity);
    encoders[1]->setPressedValue(0.5f); // noteChangeTime
    encoders[2]->setValue(0.6f); // filter
    encoders[3]->setValue(0.5f); // envelope attack
    encoders[4]->setValue(0.5f); // envelope release

    // synth parameters
    volatile float glideTime;
    volatile float detune;
    volatile float attackTime;
    volatile float releaseTime;
    volatile float lowpassCutoff;

    // encoder loop
    while (true) {

        {
            miosix::FastMutex mutex;

            // synth audio modules
            Envelope &ampEnvelope = monoSynth.getAmplifierEnvelope();
            LowpassFilter1P &lowpassFilter = monoSynth.getLowpassFilter();

            for (auto enc : encoders) enc->update(); // updating the encoder state

            // getting values from the encoders
            noteChangeTime = AudioMath::linearMap(encoders[1]->getPressedValue(), 0.0f, 1.0f, 600.0f, 50.0f);
            lowpassCutoff = encoders[2]->getValue();
            attackTime = encoders[3]->getValue();
            releaseTime = encoders[4]->getValue();

            // setting the synth
            ampEnvelope.setAttack(attackTime);
            ampEnvelope.setRelease(releaseTime);
            lowpassFilter.setCutoff(lowpassCutoff);
        }
        miosix::Thread::sleep(20);

    }
}

void synthThreadFunc() {
    // test midi queue
    size_t noteSize = sizeof(midiNotesValuesSimpleMelody) / sizeof(*midiNotesValuesSimpleMelody);
    std::vector <Midi::MidiMessage> midiQueue =
            midiMessagesFromNoteValues(midiNotesValuesSimpleMelody2, noteSize);
    float noteChangeTimeLocal;

    // synth audio modules
    {
        miosix::FastMutex mutex;
        Envelope &ampEnvelope = monoSynth.getAmplifierEnvelope();
        LowpassFilter1P &lowpassFilter = monoSynth.getLowpassFilter();
        ampEnvelope.setSustain(1.0f);
        ampEnvelope.setDecay(0.2f);
    }

    while (true) {
        for (auto midiMsg : midiQueue) {
            {
                miosix::FastMutex mutex;
                monoSynth.processMidi(midiMsg);
                noteChangeTimeLocal = noteChangeTime;
            }
            miosix::Thread::sleep(noteChangeTimeLocal);
        }
    }
}

int main() {
    // initializing the audio driver
    audioDriver.getBuffer();
    audioDriver.init();
    audioDriver.setAudioProcessable(monoSynth);

    // starting the threads
    std::thread hardwareInterfaceThread(hardwareInterfaceThreadFunc);
    std::thread synthThread(synthThreadFunc);

    // starting the audio driver (blocking function)
    audioDriver.start();
    hardwareInterfaceThread.join();
    synthThread.join();
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
