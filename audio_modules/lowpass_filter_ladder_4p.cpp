
#include "lowpass_filter_ladder_4p.h"
#include "../audio/audio_math.h"
#include <cmath>

void LowpassFilterLadder4p::process(AudioBuffer<float, 1, 256> &buffer) {
    float *p = buffer.getWritePointer(0);
    float sampleRate = getSampleRate();
    float resonanceInterpolatedValue;
    float g; // integrator gain
    float u; // input of the integrators
    float v; // input of the integrators after the gain
    float G; // paramenter G of the filter
    float S; // parameter S of the filter

    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {

        // cutoff frequency and resonance update on control rate
        cutoffFrequency.updateSampleCount(1);
        resonance.updateSampleCount(1);
        resonanceInterpolatedValue = resonance.getValue();

        // processing
        g = M_PI * cutoffFrequency.getInterpolatedValue() / sampleRate;
        G = g * g * g * g;
        S = g * g * g * states[0] + g * g * states[1] + g * states[2] + states[3];
        u = (p[i] - resonanceInterpolatedValue * S) / (1 + resonanceInterpolatedValue * G);
        // 4 single pole lowpass filters in series
        for (int state_index = 0; state_index < 4; state_index++) {
            v = (u - states[state_index]) * g / (1 + g);
            u = v + states[state_index];
            states[state_index] = u + v;
        }
//        u *= 1 + resonanceInterpolatedValue * 0.25f; // resonance gain compensation
        p[i] = u; // writing to the buffer
    }
}

void LowpassFilterLadder4p::setResonance(float resonanceAmount) {
    resonanceAmount = AudioMath::clip(resonanceAmount, 0.0f, 1.0f);
    resonanceAmount = AudioMath::linearMap(resonanceAmount, 0.0f, 1.0f,
                                           0.0f, LOWPASS_FILTER_LADDER_4P_MAX_RESONANCE);
    resonance.setValue(resonanceAmount);
}

void LowpassFilterLadder4p::setCutoffFrequency(float frequency) {
    frequency = AudioMath::clip(frequency, 0.0f, 20000.0f);
    cutoffFrequency.setValue(frequency);
}