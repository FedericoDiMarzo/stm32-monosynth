
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
    float fc; // instantaneous cutoff frequency
    float cutoffModulatorValue; // cutoff modulator instantaneous value

    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {

        // getting the values from the parameters
        resonanceInterpolatedValue = resonance.getValue();
        fc = cutoffFrequency.getInterpolatedValue();

        // modulations
        cutoffModulatorValue = (cutoffModulatorPtr != nullptr) ? cutoffModulatorPtr[i] : 0; // TODO: remove this branch
        fc += cutoffModulatorValue * cutoffModulationAmount.getInterpolatedValue();
        fc = AudioMath::clip(fc, 0.0f, 20000.0f);

        // cutoff frequency and resonance update
        cutoffFrequency.updateSampleCount(1);
        resonance.updateSampleCount(1);
        cutoffModulationAmount.updateSampleCount(1);

        // processing
        g = M_PI * fc / sampleRate;
        G = g * g * g * g;
        S = g * g * g * states[0] + g * g * states[1] + g * states[2] + states[3];
        u = (p[i] - resonanceInterpolatedValue * S) / (1 + resonanceInterpolatedValue * G);
        // 4 single pole lowpass filters in series
        for (int state_index = 0; state_index < 4; state_index++) {
            v = (u - states[state_index]) * g / (1 + g);
            u = v + states[state_index];
            states[state_index] = u + v;
        }
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

void LowpassFilterLadder4p::setCutoffModulationAmount(float normalizedValue) {
    normalizedValue = AudioMath::clip(normalizedValue, 0.0f, 1.0f);
    cutoffModulationAmount.setValue(logMapping(normalizedValue) - 20);
}

