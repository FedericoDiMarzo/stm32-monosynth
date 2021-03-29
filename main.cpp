
#include "miosix.h"
#include "../drivers/stm32f407vg_discovery/audio.h"
#include "audio/audio_processor.h"
#include "drivers/encoder.h"
#include "audio/audio_buffer.hpp"
#include "audio_processors/mono_synth.h"
#include "drivers/ssd1306.h"
#include "drivers/button.h"
#include "drivers/stm32f407vg_discovery/cs43l22dac.h"
#include "audio/audio_math.h"
#include "midi/midi.h"
#include "tests/midi_test_data.h"
#include <functional>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <array>
#include <cmath>
#include <thread>
#include <memory>


MonoSynth monoSynth;
Button button1(GPIOA, 2);
Encoder encoder1(TIM2, GPIOA, 0, 1);
Encoder encoder2(TIM1, GPIOE, 9, 11);
Encoder encoder3(TIM3, GPIOB, 4, 5);
Encoder encoder4(TIM4, GPIOD, 12, 13);

void encoderThread() {
    // encoder
    float sensitivity = 0.5;
    encoder1.setSensitivity(sensitivity);
    encoder2.setSensitivity(sensitivity);
    encoder3.setSensitivity(sensitivity);
    encoder4.setSensitivity(sensitivity);
    volatile float noteChangeTime = 1;
    volatile float glideTime = 0.005;
    volatile float encoderValue;
    encoder1.setValue(0.5);

    // encoder loop
    while (true) {
        for (auto n : midiNotesValuesInAEolian) {
            {
//                encoderValue = encoder1.getValue();
//                encoderValue = encoder2.getValue();
//                encoderValue = encoder3.getValue();
                encoderValue = encoder4.getValue();
                noteChangeTime = AudioMath::linearMap(encoder1.getValue(), 0, 1, 600, 20);
                glideTime = AudioMath::linearMap(encoder2.getValue(), 0, 1, 0.005, 0.5);
                miosix::FastMutex mutex;
                monoSynth.setNote(n);
                monoSynth.setGlide(glideTime);

            }
            miosix::Thread::sleep(noteChangeTime);
        }
    }
}

void buttonThread() {
    volatile bool button1State = false;
    static int button1Counter = 0;

    // button loop
    while (true) {
        button1.update();
        miosix::Thread::sleep(0.02);
    }
}

void sequencerThreadFunc() {
    while (true) {
        monoSynth.triggerEnvelopeOn();
        miosix::Thread::sleep(500);
        monoSynth.triggerEnvelopeOff();
        miosix::Thread::sleep(500);
    }
}

int main() {
    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.getBuffer();
    audioDriver.init(SampleRate::_44100Hz);
    audioDriver.setAudioProcessable(monoSynth);

    // starting the threads
//    std::thread task1Thread(encoderThread);
    std::thread task2Thread(buttonThread);
    std::thread sequencerThread(sequencerThreadFunc);

    // testing the envelope
    Envelope &amplifierEnvelope = monoSynth.getAmplifierEnvelope();
    amplifierEnvelope.setDelayTime(0);
    amplifierEnvelope.setAttackTime(0);
    amplifierEnvelope.setSustainTime(0.3);
    amplifierEnvelope.setReleaseTime(0);

    // starting the audio driver
    audioDriver.start();
    sequencerThread.join();

}
