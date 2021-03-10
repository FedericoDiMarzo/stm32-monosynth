
#include "miosix.h"
#include "audio.h"
#include "audio_processor.h"
#include "encoder.h"
#include "audio_buffer.h"
#include <functional>
#include <math.h>
#include <cstdint>


using namespace miosix;
using namespace std;

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
    AudioProcessorTest audioProcessorTest;
    audioProcessorTest.process();

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.getBuffer();
    audioDriver.setAudioProcessable(audioProcessorTest);
    audioDriver.init(SampleRate::_44100Hz);

    // setting the encoder1
//    Encoder encoder1(TIM4, GPIOD, 12, 13);
//    Encoder encoder2(TIM1, GPIOE, 9, 11);
    Encoder encoder3(TIM3, GPIOB, 4, 5);

    // infinite loop
    for (;;) {
//        encoder1.getValue();
//        encoder2.getValue();
        encoder3.getValue();
        sleep(0.1);
    }

}
