
#include "miosix.h"
#include "audio/audio.h"
#include "audio/audio_processor.h"
#include "drivers/encoder.h"
#include "audio/audio_buffer.hpp"
#include "audio_processors/mono_synth.h"
#include "drivers/SSD1306.h"
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

// testing an implementation of an AudioProcessor
class AudioProcessorTest : public AudioProcessor {
public:
    AudioProcessorTest() {
        for (int i = 0; i < AUDIO_DRIVER_BUFFER_SIZE; ++i) {
            sinTable[i] = sin(2 * 3.14 * i / AUDIO_DRIVER_BUFFER_SIZE);
        }
    }

    void process() override {
        auto &buffer = getBuffer();
        auto leftChannel = buffer.getWritePointer(0);
        for (unsigned int i = 0; i < getBufferSize(); ++i) {
            leftChannel[i] = sinTable[i];
        }
    }

    float sinTable[AUDIO_DRIVER_BUFFER_SIZE];
};

void task1() {

    // midi test
    uint8_t *p = noteTest;
    Midi::Parser parser;
    parser.maskChannel(2);
    while (p != std::end(noteTest)) {

    }

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.getBuffer();
    audioDriver.init(SampleRate::_44100Hz);
//    float volume = audioDriver.getVolume();
    MonoSynth monoSynth;
    AudioProcessorTest audioProcessorTest;
    audioDriver.setAudioProcessable(monoSynth);

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


    // display test
//    Ssd1306::init();

    // main loop
    volatile float noteChangeTime = 1000.0;
    volatile float glideTime = 0.005;
    float frequencies[] = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 650, 700, 750, 800};
    encoder1.setValue(0.5);
    while (true) {
        for (auto n : midiNotesValuesInAEolian) {
            noteChangeTime = AudioMath::linearMap(encoder1.getValue(), 0, 1, 600, 20);
            glideTime = AudioMath::linearMap(encoder2.getValue(), 0, 1, 0.005, 0.5);
            monoSynth.setNote(n);
            monoSynth.setGlide(glideTime);
//            audioDriver.setVolume(encoder3.getValue());
            miosix::Thread::sleep(noteChangeTime);
        }
    }

}

int main() {
    std::thread task1Thread(task1);
    task1Thread.join();
}
