
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
#include <functional>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <array>
#include <cmath>
#include <thread>
#include <memory>

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

MonoSynth monoSynth;
Button button1(GPIOA, 2);
//DoubleEncoder doubleEncoder1(GPIOA, 9);
Encoder encoder1(TIM2, GPIOA, 0, 1);
Encoder encoder2(TIM1, GPIOE, 9, 11);
Encoder encoder3(TIM3, GPIOB, 4, 5);
Encoder encoder4(TIM4, GPIOD, 12, 13);

void hardwareInterfaceThreadFunc() {
    // encoder
    float sensitivity = -0.5; // TODO: revert the hardware connections
    encoder1.setSensitivity(sensitivity);
    encoder2.setSensitivity(sensitivity);
    encoder3.setSensitivity(sensitivity);
    encoder4.setSensitivity(sensitivity);
    volatile float noteChangeTime = 1;
    volatile float glideTime = 0.005;
    volatile float attackTime = 0.01;
    volatile float decayTime = 0.1;
    volatile float sustain = 0.8;
    volatile float releaseTime = 0.2;

    encoder1.setValue(0.5);

    // encoder loop
    while (true) {
        for (auto n : midiNotesValuesOneNote) {
            Midi::Note midiNote(n, 100);
            encoder1.update();
            encoder2.update();
            encoder3.update();
            encoder4.update();
            noteChangeTime = AudioMath::linearMap(encoder1.getValue(), 0, 1, 600, 20);
//            glideTime = AudioMath::linearMap(encoder2.getValue(), 0, 1, 0.005, 0.5);
            attackTime = AudioMath::linearMap(encoder2.getValue(), 0, 1, 0.01, 0.5);
            decayTime = AudioMath::linearMap(encoder3.getValue(), 0, 1, 0.01, 0.8);
            releaseTime = AudioMath::linearMap(encoder4.getValue(), 0, 1, 0.01, 0.8);
            {
                miosix::FastMutex mutex;
//                monoSynth.setGlide(glideTime);
                auto &envelope = monoSynth.getAmplifierEnvelope();
                envelope.setAttackTime(attackTime);
                envelope.setSustainTime(decayTime);
                envelope.setReleaseTime(releaseTime);
                monoSynth.noteOn(midiNote);
            }
            miosix::Thread::sleep(noteChangeTime / 2);
            {
                miosix::FastMutex mutex;
                monoSynth.noteOff(midiNote);
            }
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
