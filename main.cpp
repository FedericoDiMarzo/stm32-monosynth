
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
#include <algorithm>
#include <math.h>
#include <cstdint>


int main() {

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.getBuffer();
    audioDriver.init(SampleRate::_44100Hz);
    Cs43l22dac::setVolume(-20);
    MonoSynth monoSynth;
    audioDriver.setAudioProcessable(monoSynth);

    // midi test
    uint8_t *p = noteTest;
    Midi::Parser parser;
    parser.maskChannel(2);
    while (p != std::end(noteTest)) {
        parser.parse(p);
    }

    // encoder
    Encoder encoder1(TIM2, GPIOA, 0, 1);
    Encoder encoder2(TIM1, GPIOE, 9, 11);
    Encoder encoder3(TIM3, GPIOB, 4, 5);
    Encoder encoder4(TIM4, GPIOD, 12, 13);

    // display test
//    Ssd1306::init();

    // main loop
    volatile float value1 = 0;
    volatile float value2 = 0;
    volatile float value3 = 0;
    volatile float value4 = 0;
    while (true) {
        value1 = encoder1.getValue();
        value2 = encoder2.getValue();
        value3 = encoder3.getValue();
        value4 = encoder4.getValue();
        sleep(0.2);

        monoSynth.setFrequency(AudioMath::linearMap(value1, 0, 1, 60, 1200));
    }

}
