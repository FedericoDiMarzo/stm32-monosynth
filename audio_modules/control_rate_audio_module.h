
#ifndef STM32_MONOSYNTH_CONTROL_RATE_AUDIO_MODULE_H
#define STM32_MONOSYNTH_CONTROL_RATE_AUDIO_MODULE_H

#include "../audio/audio_module.h"

/**
 * Audio module that contains one or more parameters that
 * must be updated at a control rate.
 * The control rate divider specifies
 * @tparam CHANNEL_NUM
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
    ControlRateAudioModule(AudioProcessor &audioProcessor, uint16_t divider = 1) :
            AudioModule<CHANNEL_NUM>(audioProcessor) {
        setControlRateDivider(divider);
    };

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
        if ((divider & (divider - 1)) != 0) { // not a power of 2
            controlRateDivider = 1;
        } else {
            controlRateDivider = divider;
        }
    };

    /**
     * This function is called inside the process to check
     * if the sample processed must trigger a control rate update.
     *
     * @param sampleNumber current process sample
     * @return true if a control rate update must be executed
     */
    inline bool isControlRateSample(int sampleNumber) {
        return (sampleNumber % AUDIO_DRIVER_BUFFER_SIZE / controlRateDivider) == 0;
    }

    /**
     * Gets the number of samples between two control rate updates
     * @return
     */
    inline int getControlRateSampleNumber() {
        return AUDIO_DRIVER_BUFFER_SIZE / controlRateDivider;
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
