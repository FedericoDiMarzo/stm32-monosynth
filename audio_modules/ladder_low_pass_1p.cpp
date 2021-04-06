
#include "ladder_low_pass_1p.h"
#include <cmath>

void LadderLPF1P::process(AudioBuffer<float, CHANNEL_NUM, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    float *p = buffer.getWritePointer(0);
    float sampleRate = getSampleRate();
    float v; // integrator gain output
    int controlRateCount = buffer.getBufferLenght() / controlRateDivider;

    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {

        // cutoff frequency update on control rate
        if (i % controlRateCount == 0) {
            cutoffFrequency.updateSampleCount(controlRateCount);
        }

        // processing (pag.77 of "The art of VA filter design")
        g = M_PI * cutoffFrequency.getInterpolatedValue() / (sampleRate * sampleRate);
        G = g / (1 + g);
        v = (p[i] - state) * G;
        p[i] = v + state;
        state = p[i] + v;
    }
}

void LadderLPF1P::setCutoffFrequency(float frequency) {
    if (frequency < 0.0f) return;
    cutoffFrequency.setValue(frequency);
}
