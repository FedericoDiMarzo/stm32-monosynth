
#include "miosix.h"
#include "audio/audio.h"
#include "audio/audio_processor.h"
#include "drivers/encoder.h"
#include "audio/audio_buffer.hpp"
#include "audio_processors/mono_synth.h"
#include "drivers/SSD1306.h"
#include "audio/cs43l22dac.h"
#include "audio/audio_math.h"
#include <functional>
#include <math.h>
#include <cstdint>


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

int main() {

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.init(SampleRate::_44100Hz);
    Cs43l22dac::setVolume(-22);


    AudioProcessorTest audioProcessorTest;
    MonoSynth monoSynth;

    // setting the audioProcessor
//        audioDriver.setAudioProcessable(audioProcessorTest);
    audioDriver.setAudioProcessable(monoSynth);


    // encoder
//    Encoder encoder1(TIM4, GPIOD, 12, 13);

    // display test
    monoSynth.setFrequency(220);
    for (;;) {
//        monoSynth.setFrequency(700);
//        miosix::Thread::sleep(time);
//        monoSynth.setFrequency(450);
//        miosix::Thread::sleep(time);
//        monoSynth.setFrequency(225);
//        miosix::Thread::sleep(time);
//        monoSynth.setFrequency(112.5);
//        miosix::Thread::sleep(time);
//        monoSynth.setFrequency(66.25);
//        miosix::Thread::sleep(time);
    }

}
