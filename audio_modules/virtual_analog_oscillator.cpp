
#include "virtual_analog_oscillator.h"
#include "../audio/audio.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_math.h"
#include "lookup_tables.h"


void VirtualAnalogOscillator::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    if (waveType == VirtualAnalogOscillatorWaveType::SAW_DPW) {
        processSawDpw(buffer);
    } else if (waveType == VirtualAnalogOscillatorWaveType::SINE)
        processSine(buffer);

}


void VirtualAnalogOscillator::processSawDpw(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    // Saw DPW algorithm
    float *p = buffer.getWritePointer(0);
    float sampleRate = getSampleRate();
    float currentParabolicSample = 0;
    float interpolatedFrequency;
    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {
        // frequency update
        interpolatedFrequency = frequency.getInterpolatedValue();
        frequency.updateSampleCount(1);

        // mapping the wrapped phase between -1 and 1
        p[i] = 2 * phase - 1;

        // parabola from the phase modulo
        p[i] = p[i] * p[i];

        // storing the parabola sample in a tmp buffer
        currentParabolicSample = p[i];

        // differentiation
        p[i] = p[i] - lastParabolicSample;

        // buffering the current parabola sample
        lastParabolicSample = currentParabolicSample;

        // gain compensation
        p[i] *= sampleRate / (4 * interpolatedFrequency);

        // updating the phase
        phase += interpolatedFrequency / sampleRate;

        // phase wrapping
        phase = (phase > 1) ? 0 : phase;
    }
}

void VirtualAnalogOscillator::processSine(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    float *p = buffer.getWritePointer(0);
    float interpolatedFrequency;
    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {
        // frequency update
        interpolatedFrequency = frequency.getInterpolatedValue();
        frequency.updateSampleCount(1);

        // LUT sine
        p[i] = AudioMath::sineLut(2 * M_PI * phase);

        // updating the phase
        phase += interpolatedFrequency / getSampleRate();

        // phase wrapping
        phase = (phase > 1) ? 0 : phase;
    }
}