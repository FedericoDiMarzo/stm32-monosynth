
#ifndef STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H
#define STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H

#include "../audio/audio.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_parameter.h"
#include "../audio/midi.h"

/**
 * AudioModule implementing a virtual analog oscillator.
 * The implementation is based on MIT CMJ302 02Valimaki 19-31
 */


enum class VirtualAnalogOscillatorWaveType {
    SAW_DPW,
    SINE
};

class VirtualAnalogOscillator : public AudioModule<1> {
public:

    VirtualAnalogOscillator(AudioProcessor &audioProcessor);

    /**
     * Sets the frequency of the oscillator.
     *
     * @param f frequency in Hz
     */
    inline void setFrequency(float f) {
        if (f < 0) return;
        frequency.setValue(f);
    }

    /**
     * Sets the glide time in seconds.
     *
     * @param time in seconds
     * @param sampleRate sample frequency
     */
    void setGlide(float time, float sampleRate) {
        frequency.setTransitionTime(time, sampleRate);
    }

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

private:

    /**
     * AudioParameter for the frequency in Hz.
     */
    AudioParameter<float> frequency;

    VirtualAnalogOscillatorWaveType waveType;

    /**
     * Phase of the oscillator, between 0 and 1.
     */
    float phase;

    /**
     * Buffer used by the saw DPW algorithm.
     */
    float lastParabolicSample;

    void processSawDpw(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer);

    void processSine(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer);


};


#endif //STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H
