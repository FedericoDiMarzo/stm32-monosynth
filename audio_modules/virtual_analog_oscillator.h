
#ifndef STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H
#define STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H

#include "../audio/audio.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include <array>

// MIT CMJ302 02Valimaki 19-31 implementation

class VirtualAnalogOscillator : public AudioModule<1> {
public:
    VirtualAnalogOscillator(float sampleRate);

    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &process() override;

private:
    float sampleRate;

    float frequency;

    int phase;

    float lastSample;

    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> buffer;

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override {};
};

#endif //STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H
