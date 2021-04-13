
#ifndef STM32_MONOSYNTH_LADDER_LOW_PASS_1P_H
#define STM32_MONOSYNTH_LADDER_LOW_PASS_1P_H

#include "../audio/audio_module.h"
#include "../audio/audio_parameter.h"
#include "control_rate_audio_module.h"


/**
 * One pole low pass IIR filter based on TPT discretization.
 *
 * Reference: Chapter 5 of "The art of VA filter design" by Vadim Zavalishin.
 */
// TODO: testing
class LowpassFilter1P : public ControlRateAudioModule<1> {
public:
    LowpassFilter1P(AudioProcessor &audioProcessor) :
            ControlRateAudioModule<1>(audioProcessor, 16),
            cutoffFrequency(3000.0f),
            state(0.0f) {};

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

    /**
     * Sets the cutoff frequency parameter.
     * The input is clipped between 20hz and 20Khz.
     *
     * @param frequency new cutoff frequency
     */
    void setCutoffFrequency(float frequency);

    /**
     * Sets the cutoff frequency parameter mapping the
     * input in a logarithmic scale.
     *
     * @param normalizedValue input between 0 and 1 mapped to the cutoff frequency
     */
    void setCutoff(float normalizedValue);


private:
    /**
     * Cutoff frequency of the filter in hertz.
     */
    AudioParameter<float> cutoffFrequency;

    /**
     * Gain of the integrator.
     */
    float g;

    /**
     * State variable, used to store the last sample of the filter input.
     */
    float state;

};


#endif //STM32_MONOSYNTH_LADDER_LOW_PASS_1P_H
