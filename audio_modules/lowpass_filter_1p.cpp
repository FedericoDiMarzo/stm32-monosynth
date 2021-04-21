
#include "lowpass_filter_1p.h"
#include "../audio/audio_math.h"
#include "lookup_tables.h"
#include <cmath>

void LowpassFilter1P::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    float *p = buffer.getWritePointer(0);
    float sampleRate = getSampleRate();
    float g; // integrator gain
    float v; // integrator input after gain

    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {

        // cutoff frequency parameter update
        cutoffFrequency.updateSampleCount(1);

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

