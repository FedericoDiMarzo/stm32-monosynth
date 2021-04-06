
#include "virtual_analog_oscillator.h"
#include "../drivers/common/audio.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_math.h"
#include "lookup_tables.h"

// default modulator
AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> VirtualAnalogOscillator::defaultNullModulator;

void VirtualAnalogOscillator::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    if (waveType == VirtualAnalogOscillatorWaveType::SAW) {
        processSawDpw(buffer);
    } else if (waveType == VirtualAnalogOscillatorWaveType::SINE)
        processSine(buffer);

}


void VirtualAnalogOscillator::processSawDpw(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    // Saw DPW algorithm
    float *out = buffer.getWritePointer(0);
    float *mod = modulator.getWritePointer(0);
    float sampleRate = getSampleRate();
    float currentParabolicSample = 0;
    float interpolatedFrequency;
    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {
        // frequency update
        interpolatedFrequency = frequency.getInterpolatedValue();
        frequency.updateSampleCount(1);

        // modulation intensity update on control rate
        if (isControlRateSample(i))
            modulationIntensity.updateSampleCount(getControlRateSampleNumber());

        // mapping the wrapped phase between -1 and 1, and adding the pitch modulation
        out[i] = 2.0f * AudioMath::rampLut(normalizedPhase + modulationIntensity.getInterpolatedValue() * mod[i]) - 1.0f;

        // parabola from the phase modulo
        out[i] = out[i] * out[i];

        // storing the parabola sample in a tmp buffer
        currentParabolicSample = out[i];

        // differentiation
        out[i] = out[i] - lastParabolicSample;

        // buffering the current parabola sample
        lastParabolicSample = currentParabolicSample;

        // gain compensation
        out[i] *= sampleRate / (4.0f * interpolatedFrequency);

        // updating the phase
        normalizedPhase += interpolatedFrequency / sampleRate;

        // phase wrapping
        normalizedPhase = (normalizedPhase > 1.0f) ? 0.0f : normalizedPhase;
    }
}

void VirtualAnalogOscillator::processSine(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    float *out = buffer.getWritePointer(0);
    float *mod = modulator.getWritePointer(0);

    float interpolatedFrequency;
    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {

        // frequency update
        interpolatedFrequency = frequency.getInterpolatedValue();
        frequency.updateSampleCount(1);

        // modulation intensity update on control rate
        if (isControlRateSample(i))
            modulationIntensity.updateSampleCount(getControlRateSampleNumber());

        // LUT sine with pitch modulation
        out[i] = AudioMath::sineLut(2.0f * M_PI * normalizedPhase +
                                  modulationIntensity.getInterpolatedValue() * mod[i]);

        // updating the phase
        normalizedPhase += interpolatedFrequency / getSampleRate();

        // phase wrapping
        normalizedPhase = (normalizedPhase > 1.0f) ? 0.0f : normalizedPhase;
    }
}