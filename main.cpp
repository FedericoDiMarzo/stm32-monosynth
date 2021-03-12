
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
    Cs43l22dac::setVolume(-22);
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
//    Encoder encoder1(TIM4, GPIOD, 12, 13);

    // display test
    monoSynth.setFrequency(220);
    for (;;) {

    }

}
