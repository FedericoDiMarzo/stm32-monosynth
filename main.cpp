
#include "miosix.h"
#include "audio/audio.h"
#include "audio/audio_processor.h"
#include "audio/audio_buffer.h"
#include "encoder.h"
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
        auto& buffer = getBuffer();
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
    audioDriver.init(SampleRate::_32000Hz);

    // encoder
    Encoder encoder1(TIM4, GPIOD, 12, 13);

    // infinite loop
    for (;;) {
        encoder1.getValue();
        sleep(0.1);
    }

}
