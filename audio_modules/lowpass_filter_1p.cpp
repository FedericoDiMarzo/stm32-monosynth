
#include "lowpass_filter_1p.h"
#include "../audio/audio_math.h"
#include "lookup_tables.h"
#include <cmath>

void LowpassFilter1P::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    float *p = buffer.getWritePointer(0);
    float sampleRate = getSampleRate();
    float v; // integrator gain output

    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {

        // cutoff frequency update on control rate
        if (isControlRateSample(i))
            cutoffFrequency.updateSampleCount(getControlRateSampleNumber());

        // processing (pag.77 of "The art of VA filter design")
        g = M_PI * cutoffFrequency.getInterpolatedValue() / sampleRate;
        v = (p[i] - state) * g / (1 + g);
        p[i] = v + state;
        state = p[i] + v;
    }
}

void LowpassFilter1P::setCutoffFrequency(float frequency) {
    frequency = AudioMath::clip(frequency, 20.0f, 20000.0f);
    cutoffFrequency.setValue(frequency);
}

void LowpassFilter1P::setCutoff(float normalizedValue) {
    normalizedValue = AudioMath::clip(normalizedValue, 0.0f, 1.0f);
    // implements a logarithmic potentiometer with values between 20 and 20k
    // TODO: move to AudioMath
    setCutoffFrequency(AudioMath::expLut(
            AUDIOMATH_LOG20 + normalizedValue * (AUDIOMATH_LOG20000 - AUDIOMATH_LOG20)));
}
