
#include "miosix.h"
#include "../drivers/common/audio.h"
#include "audio/audio_processor.h"
#include "drivers/encoder.h"
#include "drivers/double_encoder.h"
#include "audio/audio_buffer.h"
#include "audio_processors/mono_synth.h"
#include "audio_modules/envelope.h"
#include "audio_modules/lowpass_filter_1p.h"
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

// The audio driver
AudioDriver audioDriver(SampleRate::_44100Hz);

// The synth engine
MonoSynth monoSynth(audioDriver);

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
    std::array < DoubleEncoder * , 4 > encoders =
                                           {&doubleEncoder1, &doubleEncoder2, &doubleEncoder3, &doubleEncoder4};
    for (auto enc : encoders) enc->setSensitivity(EncoderSensitivity);

    encoders[0]->setPressedValue(0.5f);

    volatile float noteChangeTime = 600.0f;
    volatile float glideTime = 0.005f;
    volatile float detune = 0.005f;

    volatile float attackTime = 0.01f;
    volatile float decayTime = 0.1f;
    volatile float sustain = 0.2f;
    volatile float releaseTime = 0.1f;

    volatile float lowpassCutoff = 3000.0f;
    Envelope &ampEnvelope = monoSynth.getAmplifierEnvelope();
    LowpassFilter1P &lowpassFilter = monoSynth.getLowpassFilter();


    // encoder loop
    while (true) {

        size_t noteSize = sizeof(midiNotesValuesSimpleMelody) / sizeof(*midiNotesValuesSimpleMelody);
        std::vector <Midi::MidiMessage> midiQueue =
                midiMessagesFromNoteValues(midiNotesValuesSimpleMelody, noteSize);

        for (auto midiMsg : midiQueue) {
            for (auto enc : encoders) enc->update(); // updating the encoder state
            // getting values from the encoders
            noteChangeTime = AudioMath::linearMap(encoders[0]->getPressedValue(), 0.0f, 1.0f, 600.0f, 100.0f);
            attackTime = encoders[0]->getValue();
            decayTime = encoders[1]->getValue();
            releaseTime = encoders[2]->getValue();
            lowpassCutoff = encoders[3]->getValue();
            {
                miosix::FastMutex mutex;
                ampEnvelope.setAttack(attackTime);
                ampEnvelope.setDecay(decayTime);
                ampEnvelope.setSustain(sustain);
                ampEnvelope.setRelease(releaseTime);

                lowpassFilter.setCutoff(lowpassCutoff);

                monoSynth.processMidi(midiMsg);
            }
            miosix::Thread::sleep(noteChangeTime);
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
