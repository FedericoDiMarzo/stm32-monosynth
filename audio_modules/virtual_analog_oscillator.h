
#ifndef STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H
#define STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H

#include "../audio/audio.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_parameter.h"

/**
 * AudioModule implementing a virtual analog oscillator.
 * The implementation is based on MIT CMJ302 02Valimaki 19-31
 */
class VirtualAnalogOscillator : public AudioModule<1> {
public:

    VirtualAnalogOscillator(AudioProcessor& audioProcessor);

    /**
     * Sets the frequency of the oscillator.
     *
     * @param f frequency in Hz
     */
    void setFrequency(float f);

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;
private:

    /**
     * AudioParameter for the frequency in Hz.
     */
    AudioParameter<float> frequency;

    /**
     * Phase of the oscillator, between 0 and 1.
     */
    float phase;

    /**
     * Buffer used by the saw DPW algorithm.
     */
    float lastParabolicSample;
};

#endif //STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H
