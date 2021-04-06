
#ifndef STM32_MONOSYNTH_CONTROL_RATE_AUDIO_MODULE_H
#define STM32_MONOSYNTH_CONTROL_RATE_AUDIO_MODULE_H

#include "../audio/audio_module.h"

/**
 * Interface for an AudioModule that needs a control rate
 * update of its parameters.
 */
template<size_t CHANNEL_NUM>
class ControlRateAudioModule : public AudioModule<CHANNEL_NUM> {
public:

    /**
     * Constructor.
     *
     * @param audioDriver audio driver
     * @param divider control rate update divider
     */
    ControlRateAudioModule(AudioProcessor& audioProcessor, uint16_t divider = 1) :
            AudioModule<CHANNEL_NUM>(audioProcessor),
            controlRateDivider(divider) {};

    /**
     * Getter for the controlRateDivider attribute.
     *
     * @return controlRateDividerAttribute.
     */
    inline unsigned int getControlRateDivider() { return controlRateDivider; };

    /**
     * Setter for the controlRateDivider attribute.
     * it must be a power of two.
     *
     * @param divider value of the division
     */
    inline void setControlRateDivider(unsigned int divider) {
        if ((divider & (divider - 1)) != 0) return; // not a power of 2
        controlRateDivider = divider;
    };

    inline float test() {
        return AudioModule<CHANNEL_NUM>::getSampleRate();
    }

private:
    /**
     * Divider for the update rate of the
     * AudioParameters with respect to the sample rate.
     * It must be a power of two and less than the sampling frequency.
     */
    uint16_t controlRateDivider;
};

#endif //STM32_MONOSYNTH_CONTROL_RATE_AUDIO_MODULE_H
