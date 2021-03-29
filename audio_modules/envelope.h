
#ifndef STM32_MONOSYNTH_ENVELOPE_H
#define STM32_MONOSYNTH_ENVELOPE_H

#include "../drivers/stm32f407vg_discovery/audio.h"
#include "../audio/audio_module.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_parameter.h"

/**
 * This audio module implements a monophonic linear ADSR envelope with initial
 * delay; using the triggerOn method the envelope starts from the delay
 * phase and continues until the end of the decay phase, when triggerOff
 * is called the envelope starts the release phase and then stops.
 */
class Envelope : AudioModule<1> {
public:

    /**
     * Constructor.
     *
     * @param audioProcessor associated audioProcessor
     * @param delayTime time in seconds for the delay phase
     * @param attackTime time in seconds for the attack phase
     * @param sustainTime time in seconds for the sustain phase
     * @param releaseTime time in seconds for the release phase
     * @param sustainValue normalized value (between 0 and 1)  reached at the end of the decay phase
     * @param scale global multiplier of the envelope level
     */
    Envelope(AudioProcessor &audioProcessor,
             float delayTime = 0.0, float attackTime = 0.0,
             float sustainTime = 1.0, float releaseTime = 0.1,
             float sustainValue = 1.0, float scale = 1) :
            AudioModule<1>(audioProcessor),
            delayTime(delayTime),
            attackTime(attackTime),
            sustainTime(sustainTime),
            releaseTime(releaseTime),
            sustainValue(sustainValue),
            scale(scale),
            state(EnvelopeState::RESET),
            currentValue(0),
            timerCount(0),
            isTriggered(false),
            wasTriggered(false) {};

    /**
     * Disabling the default constructor.
     */
    Envelope() = delete;

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

    /**
     * Triggers the envelope. After this method is called the
     * envelope restarts from the delay phase.
     */
    inline void triggerOn() {
        if (wasTriggered) return; // the trigger was already on
        wasTriggered = isTriggered; // saving the last state
        isTriggered = true;
    };

    /**
     * Release the envelope trigger. After this method is called
     * the envelope if the envelope starts the release phase.
     * If the trigger was already off, this method does nothing.
     */
    inline void triggerOff() {
        if (!wasTriggered) return; // the trigger was already off
        wasTriggered = isTriggered; // saving the last state
        isTriggered = false;
    };

    /**
     * Getter for the isTriggered attribute.
     *
     * @return true if the envelope trigger is on
     */
    inline bool triggerState() { return isTriggered; };

    /**
     * getter for the delayTime attribute.
     *
     * @return delayTime
     */
    inline float getDelayTime() { return delayTime.getValue(); };

    /**
     * Getter for the attackTime attribute.
     *
     * @return attackTime
     */
    inline float getAttackTime() { return attackTime.getValue(); };

    /**
     * Getter for the sustainTime attribute.
     *
     * @return sustainTime
     */
    inline float getSustainTime() { return sustainTime.getValue(); };

    /**
     * Getter for the releaseTime attribute.
     *
     * @return releaseTime
     */
    inline float getReleaseTime() { return releaseTime.getValue(); };

    /**
     * Getter for the sustainValue attribute.
     *
     * @return sustainTime
     */
    inline float getSustainValue() { return sustainValue.getValue(); };

    /**
     * Getter for the scale attribute.
     *
     * @return scale
     */
    inline float getScale() { return scale.getValue(); };

    /**
     * Setter for the delayTime attribute.
     *
     * @param value new delayTime
     */
    inline void setDelayTime(float value) {
        value = (value < 0) ? 0 : value; // clipping
        delayTime.setValue(value);
    };

    /**
     * Setter for the attackTime attribute.
     *
     * @param value new attackTime
     */
    inline void setAttackTime(float value) {
        value = (value < 0) ? 0 : value; // clipping
        attackTime.setValue(value);
    };

    /**
     * Setter for the sustainTime attribute.
     *
     * @param value new sustainTime
     */
    inline void setSustainTime(float value) {
        value = (value < 0) ? 0 : value; // clipping
        sustainTime.setValue(value);
    };

    /**
     * Setter for the releaseTime attribute.
     *
     * @param value new releaseTime
     */
    inline void setReleaseTime(float value) {
        value = (value < 0) ? 0 : value; // clipping
        releaseTime.setValue(value);
    };

    /**
     * Setter for the sustain attribute.
     *
     * @param value new sustain
     */
    inline void setSustainValue(float value) {
        // clipping
        value = (value < 0) ? 0 : value;
        value = (value > 1) ? 1 : value;

        sustainValue.setValue(value);
    };

    /**
     * Setter for the scale attribute.
     *
     * @param value new scale
     */
    inline void setScale(float value) {
        value = (value < 0) ? 0 : value; // clipping
        scale.setValue(value);
    };

    /**
     * Destructor.
     */
    ~Envelope() {};

    /**
     * Disabling copy constructor.
     */
    Envelope(Envelope &) = delete;

    /**
     * Disabling move operator.
     * @return
     */
    Envelope &operator=(Envelope &) = delete;

    /**
     * Inner enum containing the states of the Envelope.
     */
    enum class EnvelopeState {
        RESET, // idle state
        DELAY,
        ATTACK,
        SUSTAIN,
        RELEASE
    };

private:
    /**
     * Time of the delay phase in seconds.
     */
    AudioParameter<float> delayTime;

    /**
     * Time of the attack phase in seconds.
     */
    AudioParameter<float> attackTime;

    /**
     * Time to reach the sustain value in
     * the sustain phase in seconds.
     */
    AudioParameter<float> sustainTime;

    /**
     * Time for the release phase in seconds.
     */
    AudioParameter<float> releaseTime;

    /**
     * Normalized value of the envelope at the end
     * of the sustain phase.
     */
    AudioParameter<float> sustainValue;

    /**
     * Global multiplier of the envelope level
     */
    AudioParameter<float> scale;

    /**
     * Current state of the envelope.
     */
    EnvelopeState state;

    /**
     * Current value of the envelope.
     */
    float currentValue;

    /**
     * counter in seconds used to switch the states.
     */
    float timerCount;

    /**
     * True if the trigger is on.
     */
    bool isTriggered;

    /**
     * Saves the last trigger status, used to detect a
     * note on/off.
     */
    bool wasTriggered;
};


#endif //STM32_MONOSYNTH_ENVELOPE_H
