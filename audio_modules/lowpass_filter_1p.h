
#ifndef STM32_MONOSYNTH_LADDER_LOW_PASS_1P_H
#define STM32_MONOSYNTH_LADDER_LOW_PASS_1P_H

#include "../audio/audio_module.h"
#include "../audio/audio_parameter.h"
#include "control_rate_audio_module.h"
#include "lowpass_filter.h"


/**
 * One pole low pass IIR filter based on TPT discretization (zero delay filter).
 *
 * Reference: Chapter 5 of "The art of VA filter design" by Vadim Zavalishin.
 */
// TODO: testing
class LowpassFilter1P : public ControlRateAudioModule<1>, public LowpassFilter {
public:
    LowpassFilter1P(AudioProcessor &audioProcessor) :
            ControlRateAudioModule<1>(audioProcessor, 16),
            LowpassFilter(),
            state(0.0f) {};

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

    void setCutoffFrequency(float frequency) override;


private:

    /**
     * State variable, used to store the last sample of the filter input.
     */
    float state;

};


#endif //STM32_MONOSYNTH_LADDER_LOW_PASS_1P_H
