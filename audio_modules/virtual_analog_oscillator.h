
#ifndef STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H
#define STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H

#include "../drivers/common/audio.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_parameter.h"
#include "control_rate_audio_module.h"
#include <cstdlib>
#include <cmath>

/**
 * AudioModule implementing a virtual analog oscillator.
 * The implementation is based on MIT CMJ302 02Valimaki 19-31
 */


enum class VirtualAnalogOscillatorWaveType {
    SAW,
    SQUARE, // TODO: implement square wave
    SINE,
    TRIANGLE, // TODO: implement triangle wave
};

class VirtualAnalogOscillator : public ControlRateAudioModule<1> {
public:

    VirtualAnalogOscillator(AudioProcessor &audioProcessor,
                            AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &modulator = defaultNullModulator)
            :
            ControlRateAudioModule<1>(audioProcessor, 16),
            frequency(100.0f),
            waveType(VirtualAnalogOscillatorWaveType::SAW),
            normalizedPhase(0.0f),
            lastParabolicSample(0.0f),
            modulator(modulator),
            modulationIntensity(0.0f) {

        // TODO: random seed initialization
        // random phase initialization
        normalizedPhase = static_cast <float> (rand()) / static_cast <float> (2 * M_PI);
    };

    /**
     * Sets the frequency of the oscillator.
     *
     * @param f frequency in Hz
     */
    inline void setFrequency(float f) {
        if (f < 0.0f) return;
        frequency.setValue(f);
    }

    /**
     * Sets the glide time in seconds.
     *
     * @param time in seconds
     * @param sampleRate sample frequency
     */
    inline void setGlide(float time, float sampleRate) {
        frequency.setTransitionTime(time, sampleRate);
    }

    /**
     * Sets the pitch modulation intensity.
     *
     * @param normalizedValue value between 0 and 1
     */
     // TODO: test
    inline void setModulatorIntensity(float normalizedValue) {
        // TODO: check the modulation intensity mapping range
        normalizedValue = AudioMath::clip(normalizedValue, 0.0f, 1.0f);
        modulationIntensity.setValue(AudioMath::linearMap(normalizedValue, 0.0f, 1.0f, 0.0f, 0.2f));
    }

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

private:

    /**
     * AudioParameter for the frequency in Hz.
     */
    AudioParameter<float> frequency;

    /**
     * Output waveform.
     */
    VirtualAnalogOscillatorWaveType waveType;

    /**
     * Phase of the oscillator, between 0 and 1.
     */
    float normalizedPhase;

    /**
     * Buffer used by the saw DPW algorithm.
     */
    float lastParabolicSample;

    /**
     * Buffer used to store the modulator signal.
     * This signal is used to modulate the pitch of
     * the oscillator.
     */
    // TODO: implement pitch modulation
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &modulator;

    /**
     * Amount of pitch modulation.
     */
    AudioParameter<float> modulationIntensity;

    /**
     * Modulator with constant 0 values used as default.
     */
    static AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> defaultNullModulator;

    /**
     * Method used to render a sawtooth wave based on
     * differentiation of a parabolic waveform.
     *
     * @param buffer output
     */
    void processSawDpw(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer);

    /**
     * Method used to render a sine wave based
     * on a lookup table
     *
     * @param buffer output
     */
    void processSine(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer);

};


#endif //STM32_MONOSYNTH_VIRTUAL_ANALOG_OSCILLATOR_H
