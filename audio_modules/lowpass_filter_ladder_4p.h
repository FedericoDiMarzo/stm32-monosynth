#ifndef STM32_MONOSYNTH_LOWPASS_FILTER_LADDER_4P_H
#define STM32_MONOSYNTH_LOWPASS_FILTER_LADDER_4P_H

#include "../audio/audio_module.h"
#include "control_rate.h"
#include "../audio/audio_parameter.h"
#include "filter.h"
#include <array>

/**
 * Control rate divider for the envelope modulation.
 * It must be a power of 2.
 */
#define LOWPASS_FILTER_LADDER_4P_CONTROL_RATE_DIVIDER 32

/**
 * Maximum amount of the resonance.
 */
#define LOWPASS_FILTER_LADDER_4P_MAX_RESONANCE 3.2f

class LowpassFilterLadder4p : public AudioModule<1>, public ControlRate, public Filter {
public:
    LowpassFilterLadder4p(AudioProcessor &audioProcessor) :
            AudioModule<1>(audioProcessor),
            ControlRate(LOWPASS_FILTER_LADDER_4P_CONTROL_RATE_DIVIDER),
            Filter(),
            resonance(0.0f),
            states{0.0f, 0.0f, 0.0f, 0.0f},
            cutoffModulatorPtr(nullptr),
            cutoffModulationAmount(0.0f) {};

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

    void setCutoffFrequency(float frequency) override;

    /**
     * Sets the resonance of the filter.
     * The input is mapped between 0 and 3.5.
     *
     * @param normalizedValue amount of resonance of the filter, between 0 and 1
     */
    void setResonance(float normalizedValue);

    /**
     * Sets the pointer to the signal used to modulate the cutoff frequency.
     *
     * @param readPtr pointer to the beginning of the raw buffer of the modulator for the cutoff frequency
     */
    inline void setCutoffModulatorPtr(const float *readPtr) { cutoffModulatorPtr = readPtr; };

    /**
     * Sets the amount of modulation for the filter cutoff.
     *
     * @param normalizedValue value between 0 and 1
     */
    void setCutoffModulationAmount(float normalizedValue);

    /**
     * Getter for the parameter cutoffModulationAmount.
     *
     * @return cutoff modulation amount parameter
     */
    inline AudioParameter<float> getModulatorAmt() { return cutoffModulationAmount; };


private:

    /**
     * Resonance of the filter.
     * It can assume values between 0 and LOWPASS_FILTER_LADDER_4P_MAX_RESONANCE.
     * Because of the linear design, this filter
     * doesn't allow for auto-oscillation.
     */
    AudioParameter<float> resonance;

    /**
     * State variables of the integrators
     */
    std::array<float, 4> states;

    /**
     * Pointer to the cutoff frequency modulator envelope raw buffer.
     */
    const float *cutoffModulatorPtr;

    /**
     * Amount of the modulation for the cutoff frequency in Hz.
     */
    AudioParameter<float> cutoffModulationAmount;
};


#endif //STM32_MONOSYNTH_LOWPASS_FILTER_LADDER_4P_H
