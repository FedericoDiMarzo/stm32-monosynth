
#include "miosix.h"
#include "audio.h"
#include "audio_processor.h"
#include "encoder.h"
#include <functional>
#include <math.h>

using namespace miosix;
using namespace std;

// testing an implementation of an AudioProcessor
#define BUFFSIZE 256

class AudioProcessorTest : public AudioProcessor {
public:
    AudioProcessorTest() {
        for (int i = 0; i < BUFFSIZE; ++i) {
            sinTable[i] = 4095 * ((sin(2 * 3.14 * i / BUFFSIZE) + 1) / 2);
        }

    }

    inline void process() override {
        auto *b = getBuffer();
        for (int i = 0; i < getBufferSize(); ++i) {
            b[i] = sinTable[i];
        }
    }

    unsigned int sinTable[BUFFSIZE];
};

int main() {
    AudioProcessorTest audioProcessorTest;

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.init(SampleRate::_44100Hz);

    // setting the audio processor
    audioDriver.setAudioProcessable(audioProcessorTest);

    // setting the encoder
//    Encoder encoder1;
    Encoder::testStaticRotary();

    // infinite loop
    for (;;) {
//        encoder1.getValue();
        sleep(0.1);
    }

}
