
#include "miosix.h"
#include "../drivers/stm32f407vg_discovery/audio.h"
#include "audio/audio_processor.h"
#include "drivers/encoder.h"
#include "drivers/double_encoder.h"
#include "audio/audio_buffer.h"
#include "audio_processors/mono_synth.h"
#include "drivers/ssd1306.h"
#include "drivers/button.h"
#include "drivers/stm32f407vg_discovery/cs43l22dac.h"
#include "audio/audio_math.h"
#include "midi/midi.h"
#include "tests/midi_test_data.h"
#include "tests/hardware_tests/encoder_test.h"
#include "tests/hardware_tests/double_encoder_test.h"
#include "tests/hardware_tests/button_test.h"
#include "drivers/hardware_configuration.h"
#include <functional>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <array>
#include <cmath>
#include <thread>
#include <memory>
#include <vector>

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
//#define TEST_DOUBLE_ENCODERS
///////////////////////////
///////////////////////////

#ifndef TESTING_HARDWARE

// The synth engine
MonoSynth monoSynth;

// double encoders
DoubleEncoder doubleEncoder1(DoubleEncoder1Timer, DoubleEncoder1PortEnc,
                             DoubleEncoder1PinAEnc, DoubleEncoder1PinBEnc,
                             DoubleEncoder1PortBut, DoubleEncoder1PinBut);
DoubleEncoder doubleEncoder2(DoubleEncoder2Timer, DoubleEncoder2PortEnc,
                             DoubleEncoder2PinAEnc, DoubleEncoder2PinBEnc,
                             DoubleEncoder2PortBut, DoubleEncoder2PinBut);
DoubleEncoder doubleEncoder3(DoubleEncoder3Timer, DoubleEncoder3PortEnc,
                             DoubleEncoder3PinAEnc, DoubleEncoder3PinBEnc,
                             DoubleEncoder3PortBut, DoubleEncoder3PinBut);
DoubleEncoder doubleEncoder4(DoubleEncoder4Timer, DoubleEncoder4PortEnc,
                             DoubleEncoder4PinAEnc, DoubleEncoder4PinBEnc,
                             DoubleEncoder4PortBut, DoubleEncoder4PinBut);

std::vector<Midi::MidiMessage> midiMessagesFromNoteValues(uint8_t *noteArray, size_t size) {
    std::vector<Midi::MidiMessage> midiQueue;
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
    std::array < DoubleEncoder *, 4 > encoders =
            {&doubleEncoder1, &doubleEncoder2, &doubleEncoder3, &doubleEncoder4};
    float sensitivity = -0.5; // TODO: revert the hardware connections
    for (auto enc : encoders) {
        enc->setSensitivity(sensitivity);
        enc->setValue(0.5);
    }

    volatile float noteChangeTime = 1;
    volatile float glideTime = 0.005;
    volatile float detune = 0.005;

    volatile float attackTime = 0.01;
    volatile float decayTime = 0.1;
    volatile float sustain = 0.8;
    volatile float releaseTime = 0.1;


    // encoder loop
    while (true) {
        size_t noteSize = sizeof(midiNotesValuesSimpleMelody)/sizeof(*midiNotesValuesSimpleMelody);
        std::vector<Midi::MidiMessage> midiQueue =
                midiMessagesFromNoteValues(midiNotesValuesSimpleMelody, noteSize);

        for (auto midiMsg : midiQueue) {
            for (auto enc : encoders) enc->update(); // updating the encoder state
            noteChangeTime = AudioMath::linearMap(encoders[0]->getValue(), 0, 1, 600, 20);
            glideTime = AudioMath::linearMap(encoders[1]->getValue(), 0, 1, 0.005, 0.5);
            detune = encoders[2]->getValue();
//            attackTime = AudioMath::linearMap(encoder2.getValue(), 0, 1, 0.01, 0.5);
//            decayTime = AudioMath::linearMap(encoder3.getValue(), 0, 1, 0.01, 0.8);
            releaseTime = AudioMath::linearMap(encoders[3]->getValue(), 0, 1, 0.01, 0.8);
            {
                miosix::FastMutex mutex;
                monoSynth.setGlide(glideTime);
                monoSynth.setDetune(detune);
//                auto &envelope = monoSynth.getAmplifierEnvelope();
//                envelope.setAttackTime(attackTime);
//                envelope.setSustainTime(decayTime);
//                envelope.setReleaseTime(releaseTime);
                monoSynth.processMidi(midiMsg);
            }
            miosix::Thread::sleep(noteChangeTime);
        }
    }
}


int main() {
    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.getBuffer();
    audioDriver.init(SampleRate::_44100Hz);
    audioDriver.setAudioProcessable(monoSynth);

    // starting the threads
    std::thread hardwareInterfaceThread(hardwareInterfaceThreadFunc);

    // starting the audio driver (blocking function)
    audioDriver.start();
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
