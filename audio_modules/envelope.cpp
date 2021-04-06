
#include "envelope.h"
#include "../drivers/stm32f407vg_discovery/audio.h"
#include "../audio/audio_math.h"
#include "../audio/audio_module.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_math.h"


void Envelope::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    float *p = buffer.getWritePointer(0);

    // envelope generation
    for (uint32_t i = 0; i < AUDIO_DRIVER_BUFFER_SIZE; i++) {
        count += alpha[state]; // increasing the counter

        // TODO: debug
        if (state == 1) {
            state = state;
        }

        if (count > 1.0f
            && state != ENVELOPE_STATE_SUSTAIN
            && state != ENVELOPE_STATE_RELEASE) {
            count = 0.0f; // resetting the counter
            state = (state + 1) % ENVELOPE_STATE_COUNT;
        }
        lastOutput = alpha[state] * (gateSignals[state] - lastOutput) + lastOutput;
        p[i] = lastOutput; // writing on the buffer
    }
}

void Envelope::setAlpha(float tau, uint8_t index) {
    if (tau <= 1e-8f || index > 3) return; // division by zero and index check
    alpha[index] = 1.0f / tau / getSampleRate();
}

float Envelope::boundedTau(float normalizedValue) {
    // TODO: move clipping to audioMath
    normalizedValue = (normalizedValue < 0.0f)? 0.0f : normalizedValue;
    normalizedValue = (normalizedValue > 1.0f)? 1.0f : normalizedValue;
    return AudioMath::linearMap(normalizedValue, 0, 1, ENVELOPE_MIN_TAU, ENVELOPE_MAX_TAU);
}

void Envelope::setSustain(float normalizedValue) {
    // TODO: move clipping to audioMath
    normalizedValue = (normalizedValue < 0.0f)? 0.0f : normalizedValue;
    normalizedValue = (normalizedValue > 1.0f)? 1.0f : normalizedValue;
    gateSignals[ENVELOPE_STATE_SUSTAIN] = normalizedValue;
}

