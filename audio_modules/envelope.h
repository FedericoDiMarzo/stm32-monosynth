
#ifndef STM32_MONOSYNTH_ENVELOPE_H
#define STM32_MONOSYNTH_ENVELOPE_H

#include "../drivers/stm32f407vg_discovery/audio.h"
#include "control_rate.h"
#include "../audio/audio_module.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_parameter.h"
#include <array>

#define ENVELOPE_STATE_ATTACK 0
#define ENVELOPE_STATE_DECAY 1
#define ENVELOPE_STATE_SUSTAIN 2
#define ENVELOPE_STATE_RELEASE 3
#define ENVELOPE_STATE_COUNT 4
#define ENVELOPE_MIN_TAU 0.001f
#define ENVELOPE_MAX_TAU 1.0f
// TODO: fine tune max and min tau


class Envelope : public AudioModule<1>, public ControlRate {
public:

    Envelope(AudioProcessor &audioProcessor) :
            AudioModule<1>(audioProcessor),
            lastOutput(0.0f),
            count(0.0f),
            state(ENVELOPE_STATE_RELEASE),
            alpha{ENVELOPE_MIN_TAU, 0.001f, 0.002f, 0.0001f}, // TODO: find a better initialization
            gateSignals{0.0f, 1.0f, 0.4f, 0.0f} // steady state values
    {};

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

    /**
     * Sets the attack of the envelope.
     *
     * @param normalizedValue a value between 0 and 1
     */
    inline void setAttack(float normalizedValue) { setAlpha(boundedTau(normalizedValue), 1); };

    /**
     * Sets the decay of the envelope.
     *
     * @param normalizedValue a value between 0 and 1
     */
    inline void setDecay(float normalizedValue) { setAlpha(boundedTau(normalizedValue), 2); };

    /**
     * Sets the release of the envelope.
     *
     * @param normalizedValue a value between 0 and 1
     */
    inline void setRelease(float normalizedValue) { setAlpha(boundedTau(normalizedValue), 3); };

    /**
     * Sets the sustain of the envelope.
     *
     * @param normalizedValue a value between 0 and 1
     */
    void setSustain(float normalizedValue);

    /**
     * Switch to the attack state.
     */
    inline void triggerOn() { state = ENVELOPE_STATE_ATTACK; };

    /**
     * Switch to the release state.
     */
    inline void triggerOff() { state = ENVELOPE_STATE_RELEASE; };

    /**
     * Getter for the isTriggered attribute.
     *
     * @return true if the envelope trigger is on
     */
    inline bool isTriggered() { return state != 3; };


private:

    /**
     * Last output sample of the envelope.
     */
    float lastOutput;

    /**
     * counter used to switch the states.
     */
    float count;

    /**
     * Used to keep track of the envelope state.
     * 0: Attack
     * 1: Decay
     * 2: Sustain
     * 3: Release (initial state)
     */
    uint8_t state;

    /**
     * Alpha parameters of the exponential moving
     * average filter. One for each state.
     */
    std::array<float, 4> alpha;

    /**
     * Gate signals feed into the exponential moving
     * average filter. These signals are smoothed by the
     * filter to obtain a piecewise exponential curve.
     */
    std::array<float, 4> gateSignals;

    /**
     * Set one specific alpha parameter based on
     * the time constant value.
     * alpha = 1 / (tau * sampleRate)
     *
     * @param tau time constant value
     * @param index alpha index
     */
    void setAlpha(float tau, uint8_t index);

    /**
     * Returns a time constant value bounded between
     * ENVELOPE_MIN_TAU and ENVELOPE_MAX_TAU
     *
     * @param normalizedValue a value between 0 and 1
     * @return time constant
     */
    float boundedTau(float normalizedValue);
};


#endif //STM32_MONOSYNTH_ENVELOPE_H
