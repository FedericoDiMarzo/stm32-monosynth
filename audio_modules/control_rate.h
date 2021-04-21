
#ifndef STM32_MONOSYNTH_CONTROL_RATE_H
#define STM32_MONOSYNTH_CONTROL_RATE_H


/**
 * Audio module that contains one or more parameters that
 * must be updated at a control rate.
 * The control rate divider specifies
 *
 * @tparam CHANNEL_NUM number of channels of the AudioModule
 */
class ControlRate {
public:

    /**
     * Constructor.
     *
     * @param sampleInterval samples between two control rate updates
     */
    ControlRate(uint32_t sampleInterval = 16) :
            sampleInterval(sampleInterval) {};

    /**
     * Sets the samples between two control rate updates
     *
     * @param samples new sampleInterval
     */
    inline void setSampleInterval(uint32_t samples) {
        sampleInterval = samples;
    };

    /**
     * This function is called inside the process to check
     * if the sample processed must trigger a control rate update.
     *
     * @param sampleNumber current process sample
     * @return true if a control rate update must be executed
     */
    inline bool isControlRateSample(int sampleNumber) {
        return (sampleNumber % sampleInterval) == 0;
    }

    /**
     * Gets the number of samples between two control rate updates
     *
     * @return number of samples
     */
    inline int getSampleInterval() {
        return sampleInterval;
    }

private:
    /**
     * Number of samples before a control rate updates.
     */
    uint32_t sampleInterval;
};

#endif //STM32_MONOSYNTH_CONTROL_RATE_H
