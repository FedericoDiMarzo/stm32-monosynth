
#include "miosix.h"
#include "audio/audio.h"
#include "audio/audio_processor.h"
#include "drivers/encoder.h"
#include "audio/audio_buffer.hpp"
#include "audio_processors/mono_synth.h"
#include "drivers/ssd1306.h"
#include "drivers/button.h"
#include "audio/cs43l22dac.h"
#include "audio/audio_math.h"
#include "audio/midi.h"
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


void encoderThread() {
    // encoder
    Encoder encoder1(TIM2, GPIOA, 0, 1);
    Encoder encoder2(TIM1, GPIOE, 9, 11);
    Encoder encoder3(TIM3, GPIOB, 4, 5);
    Encoder encoder4(TIM4, GPIOD, 12, 13);

    float sensitivity = 0.5;
    encoder1.setSensitivity(sensitivity);
    encoder2.setSensitivity(sensitivity);
    encoder3.setSensitivity(sensitivity);
    encoder4.setSensitivity(sensitivity);

    volatile float noteChangeTime = 1000.0;
    volatile float glideTime = 0.005;
//    float frequencies[] = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 650, 700, 750, 800};
    encoder1.setValue(0.5);
    // encoder loop
    while (true) {
        for (auto n : midiNotesValuesInAEolian) {
            noteChangeTime = AudioMath::linearMap(encoder1.getValue(), 0, 1, 600, 20);
            glideTime = AudioMath::linearMap(encoder2.getValue(), 0, 1, 0.005, 0.5);
            {
                miosix::FastMutex mutex;
                monoSynth.setNote(n);
                monoSynth.setGlide(glideTime);
            }
            miosix::Thread::sleep(noteChangeTime);
        }
    }
}

void buttonThread() {
    Button button1(GPIOA, 2);
    volatile bool button1State = false;
    static int button1Counter = 0;
    button1.setCallback([]() {
        button1Counter++;
    });

    // button loop
    while (true) {
        button1.update();
        button1State = button1.isPressed();
        miosix::Thread::sleep(0.02);
    }
}

int main() {

    // midi test
    uint8_t *p = noteTest;
    Midi::Parser parser;
    parser.maskChannel(2);
    while (p != std::end(noteTest)) {
        parser.parse(p);
    }

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.getBuffer();
    audioDriver.init(SampleRate::_44100Hz);
    audioDriver.setAudioProcessable(monoSynth);

    // starting the threads
//    std::thread task1Thread(encoderThread);
    std::thread task2Thread(buttonThread);
//    task1Thread.join();
    task2Thread.join();
}
