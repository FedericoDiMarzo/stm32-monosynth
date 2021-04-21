

#ifndef STM32_MONOSYNTH_FILTER_H
#define STM32_MONOSYNTH_FILTER_H

#include "lookup_tables.h"
#include "../audio/audio_parameter.h"

/**
 * Interface describing a lowpass filter.
 */
class Filter {
public:
    Filter() : cutoffFrequency(3000.0f) {}

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
        setCutoffFrequency(logMapping(normalizedValue));
    };

    /**
     * Getter for the attribute cutoffFrequency.
     *
     * @return cutoff frequency audio parameter
     */
    inline AudioParameter<float> getCutoffFrequency() { return cutoffFrequency; };


protected:
    /**
     * Cutoff frequency of the filter in hertz.
     */
    AudioParameter<float> cutoffFrequency;

    /**
     * Maps logarithmically an input bounded from 0 to 1
     * to a value between 20 and 20k
     *
     * @param normalizedValue value between 0 and 1
     * @return value between 20 and 20k mapped logarithmically
     */
    inline float logMapping(float normalizedValue) {
        return AudioMath::expLut(AUDIOMATH_LOG20 + normalizedValue * (AUDIOMATH_LOG20000 - AUDIOMATH_LOG20));
    }

};


#endif //STM32_MONOSYNTH_FILTER_H
