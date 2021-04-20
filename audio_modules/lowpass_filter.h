

#ifndef STM32_MONOSYNTH_LOWPASS_FILTER_H
#define STM32_MONOSYNTH_LOWPASS_FILTER_H

#include "lookup_tables.h"

/**
 * Interface describing a lowpass filter.
 */
class LowpassFilter {
public:
    LowpassFilter() : cutoffFrequency(3000.0f) {}

    /**
     * Sets the cutoff frequency parameter.
     * The input is clipped between 20hz and 20Khz.
     *
     * @param frequency new cutoff frequency
     */
    virtual void setCutoffFrequency(float frequency) = 0;

    /**
     * Sets the cutoff frequency parameter mapping the
     * input in a logarithmic scale.
     *
     * @param normalizedValue input between 0 and 1 mapped to the cutoff frequency
     */
    inline void setCutoff(float normalizedValue) {
        normalizedValue = AudioMath::clip(normalizedValue, 0.0f, 1.0f);
        // implements a logarithmic potentiometer with values between 20 and 20k
        setCutoffFrequency(AudioMath::expLut(
                AUDIOMATH_LOG20 + normalizedValue * (AUDIOMATH_LOG20000 - AUDIOMATH_LOG20)));
    };


protected:
    /**
     * Cutoff frequency of the filter in hertz.
     */
    AudioParameter<float> cutoffFrequency;

};


#endif //STM32_MONOSYNTH_LOWPASS_FILTER_H
