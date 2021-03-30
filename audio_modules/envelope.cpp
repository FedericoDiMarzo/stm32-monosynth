
#include "envelope.h"
#include "../drivers/stm32f407vg_discovery/audio.h"
#include "../audio/audio_math.h"
#include "../audio/audio_module.h"
#include "../audio/audio_buffer.h"

void Envelope::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    float *p = buffer.getWritePointer(0);
    bool noteOn;
    bool noteOff;
    float timeOfCurrentPhase;
    float mixInterpolation;




    // getting the scale updated value
    float currentScale = scale.getInterpolatedValue();

    // envelope generation
    for (uint32_t i = 0; i < AUDIO_DRIVER_BUFFER_SIZE; i++) {
        // updating all the parameters
        // TODO: control rate setup
        delayTime.updateSampleCount(1);
        attackTime.updateSampleCount(1);
        sustainTime.updateSampleCount(1);
        releaseTime.updateSampleCount(1);
        sustainValue.updateSampleCount(1);
        scale.updateSampleCount(1);

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


        // state pattern //
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
            p[i] = AudioMath::linearInterpolation(0, currentScale, mixInterpolation);

        } else if (state == EnvelopeState::SUSTAIN) {
            // sustain generation
            timeOfCurrentPhase = sustainTime.getInterpolatedValue();
            p[i] = AudioMath::linearInterpolation(currentScale,
                                                  currentScale * sustainValue.getInterpolatedValue(),
                                                  mixInterpolation);

        } else if (state == EnvelopeState::RELEASE) {
            // state change
            if (timerCount >= releaseTime.getInterpolatedValue()) {
                state = EnvelopeState::RESET;
                timerCount = 0;
            }
            // release generation
            p[i] = AudioMath::linearInterpolation(currentScale * sustainValue.getInterpolatedValue(), 0,
                                                  mixInterpolation);
        }

        // updating the timer
        timerCount += 1 / getSampleRate() * AUDIO_DRIVER_BUFFER_SIZE;
    }
}