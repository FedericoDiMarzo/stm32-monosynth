
#ifndef STM32_MONOSYNTH_CONTROL_RATE_H
#define STM32_MONOSYNTH_CONTROL_RATE_H


/**
 * Interface for an AudioModule that needs a control rate
 * update of its parameters.
 */
class ControlRate {
public:

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


private:
    /**
     * Divider for the update rate of the
     * AudioParameters with respect to the sample rate.
     * It must be a power of two and less than the sampling frequency.
     */
    unsigned int controlRateDivider;
};

#endif //STM32_MONOSYNTH_CONTROL_RATE_H
