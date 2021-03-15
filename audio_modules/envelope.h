
#ifndef STM32_MONOSYNTH_ENVELOPE_H
#define STM32_MONOSYNTH_ENVELOPE_H

#include "../audio/audio.h"
#include "../audio/audio_module.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_parameter.h"


class Envelope : AudioModule<1> {
public:
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

    Envelope() = delete;

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) override;

    inline void triggerOn() {
        wasTriggered = isTriggered;
        isTriggered = true;
    };

    inline void triggerOff() {
        wasTriggered = isTriggered;
        isTriggered = true;
    };

    inline bool triggerState() { return isTriggered; };

    inline float getDelayTime() { return delayTime.getValue(); };

    inline float getAttackTime() { return attackTime.getValue(); };

    inline float getSustainTime() { return sustainTime.getValue(); };

    inline float getReleaseTime() { return releaseTime.getValue(); };

    inline float getSustainValue() { return sustainValue.getValue(); };

    inline float getScale() { return scale.getValue(); };

    inline void setDelayTime(float value) { delayTime.setValue(value); };

    inline void setAttackTime(float value) { attackTime.setValue(value); };

    inline void setSustainTime(float value) { sustainTime.setValue(value); };

    inline void setReleaseTime(float value) { releaseTime.setValue(value); };

    inline void setSustainValue(float value) { sustainValue.setValue(value); };

    inline void setScale(float value) { scale.setValue(value); };

    Envelope(Envelope &) = delete;

    Envelope &operator=(Envelope &) = delete;

    enum class EnvelopeState {
        RESET,
        DELAY,
        ATTACK,
        SUSTAIN,
        RELEASE
    };

private:
    AudioParameter<float> delayTime;
    AudioParameter<float> attackTime;
    AudioParameter<float> sustainTime;
    AudioParameter<float> releaseTime;
    AudioParameter<float> sustainValue;
    AudioParameter<float> scale;
    EnvelopeState state;
    float currentValue;
    float timerCount;
    bool isTriggered;
    bool wasTriggered;
};


#endif //STM32_MONOSYNTH_ENVELOPE_H
