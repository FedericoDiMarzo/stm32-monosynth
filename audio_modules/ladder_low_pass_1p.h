
#ifndef STM32_MONOSYNTH_LADDER_LOW_PASS_1P_H
#define STM32_MONOSYNTH_LADDER_LOW_PASS_1P_H

#include "../audio/audio_module.h"
#include "../audio/audio_parameter.h"
#include "control_rate_audio_module.h"


/**
 * One pole ladder low pass filter  based on TPT discretization (IIR design).
 *
 * Reference: Chapter 5 of "The art of VA filter design" by Vadim Zavalishin.
 */
// TODO: testing
class LadderLPF1P : public ControlRate<1> {
public:
    LadderLPF1P(AudioProcessor& audioProcessor) :
            ControlRate<1>(audioProcessor, 16),
            cutoffFrequency(3000),
            state(0),
            G(0) {}; // TODO: check init values correctness

    void process(AudioBuffer<float, CHANNEL_NUM, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

    /**
     * Sets the cutoff frequency parameter.
     *
     * @param frequency new cutoff frequency
     */
    void setCutoffFrequency(float frequency);


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

    /**
     * Gain parameter of the filter equation.
     *
     * G = g/(1+g)
     */
    float G;

};


#endif //STM32_MONOSYNTH_LADDER_LOW_PASS_1P_H
