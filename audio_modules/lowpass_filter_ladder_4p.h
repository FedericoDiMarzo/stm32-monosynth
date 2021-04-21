#ifndef STM32_MONOSYNTH_LOWPASS_FILTER_LADDER_4P_H
#define STM32_MONOSYNTH_LOWPASS_FILTER_LADDER_4P_H

#include "../audio/audio_module.h"
#include "../audio/audio_parameter.h"
#include "filter.h"
#include <array>

#define LOWPASS_FILTER_LADDER_4P_MAX_RESONANCE 3.5f

class LowpassFilterLadder4p : public AudioModule<1>, public Filter {
public:
    LowpassFilterLadder4p(AudioProcessor &audioProcessor) :
            AudioModule<1>(audioProcessor),
            Filter(),
            resonance(0.0f),
            states{0.0f, 0.0f, 0.0f, 0.0f} {};

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

    void setCutoffFrequency(float frequency) override;

    /**
     * Sets the resonance of the filter.
     * The input is mapped between 0 and 3.5.
     *
     * @param normalizedValue amount of resonance of the filter, between 0 and 1
     */
    void setResonance(float normalizedValue);


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
};


#endif //STM32_MONOSYNTH_LOWPASS_FILTER_LADDER_4P_H
