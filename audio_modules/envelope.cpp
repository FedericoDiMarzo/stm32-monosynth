
#include "envelope.h"
#include "../audio/audio.h"
#include "../audio/audio_math.h"
#include "../audio/audio_module.h"
#include "../audio/audio_buffer.h"

void Envelope::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    float *p = buffer.getWritePointer(0);
    bool noteOn;
    bool noteOff;
    float timeOfCurrentPhase;
    float mixInterpolation;


    // updating all the parameters
    delayTime.updateSampleCount(AUDIO_DRIVER_BUFFER_SIZE);
    attackTime.updateSampleCount(AUDIO_DRIVER_BUFFER_SIZE);
    sustainTime.updateSampleCount(AUDIO_DRIVER_BUFFER_SIZE);
    releaseTime.updateSampleCount(AUDIO_DRIVER_BUFFER_SIZE);
    sustainValue.updateSampleCount(AUDIO_DRIVER_BUFFER_SIZE);
    scale.updateSampleCount(AUDIO_DRIVER_BUFFER_SIZE);

    // envelope generation
    for (uint32_t i = 0; i < AUDIO_DRIVER_BUFFER_SIZE; i++) {
        // state switch from note on/off
        noteOn = (isTriggered != wasTriggered) && (isTriggered);
        noteOff = (isTriggered != wasTriggered) && (!isTriggered);
        if (noteOn) {
            // a note on triggers the delay phase
            state = EnvelopeState::DELAY;
            timerCount = 0;
        } else if (noteOff) {
            // a note off triggers the release phase
            state = EnvelopeState::RELEASE;
            timerCount = 0;
        }


        // break if the envelope is not active
        if (state == EnvelopeState::RESET) {
            break;
        }


        // state pattern
        if (state == EnvelopeState::DELAY) {
            // state change
            if (timerCount >= delayTime.getInterpolatedValue()) {
                state = EnvelopeState::ATTACK;
                timerCount = 0;
            }
            // delay generation
            p[i] = 0;

        } else if (state == EnvelopeState::ATTACK) {
            // state change
            timeOfCurrentPhase = attackTime.getInterpolatedValue();
            if (timerCount >= timeOfCurrentPhase) {
                state = EnvelopeState::SUSTAIN;
                timerCount = 0;
            }
            // attack generation
            mixInterpolation = timerCount / timeOfCurrentPhase;
            p[i] = AudioMath::linearInterpolation(0, scale.getInterpolatedValue(), mixInterpolation);

        } else if (state == EnvelopeState::SUSTAIN) {
            // sustain generation
            p[i] = AudioMath::linearInterpolation(scale.getInterpolatedValue(),
                                                  sustainValue.getInterpolatedValue(), mixInterpolation);

        } else if (state == EnvelopeState::RELEASE) {
            // state change
            if (timerCount >= releaseTime.getInterpolatedValue()) {
                state = EnvelopeState::RESET;
                isTriggered = false;
                wasTriggered = false;
                timerCount = 0;
            }
            // release generation
            p[i] = AudioMath::linearInterpolation(sustainValue.getInterpolatedValue(), 0, mixInterpolation);
        }
    }
}