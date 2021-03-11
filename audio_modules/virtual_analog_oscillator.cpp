
#include "virtual_analog_oscillator.h"
#include "../audio/audio.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../audio/audio_processor.h"

VirtualAnalogOscillator::VirtualAnalogOscillator(AudioProcessor &audioProcessor)
        :
        AudioModule<1>(audioProcessor),
        frequency(2000.0),
        phase(0.0),
        lastParabolicSample(0.0) {

}

void VirtualAnalogOscillator::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer) {
    // Saw DPW algorithm
    float *p = buffer.getWritePointer(0);
    float sampleRate = getSampleRate();
    float currentParabolicSample = 0;
    for (uint32_t i = 0; i < buffer.getBufferLength(); i++) {
        p[i] = 2 * phase - 1;
        p[i] = p[i] * p[i];
        currentParabolicSample = p[i];
        p[i] = p[i] - lastParabolicSample;
        lastParabolicSample = currentParabolicSample;
        p[i] *= sampleRate / (4 * frequency.getValue());
        phase += frequency.getValue() / sampleRate;
        phase = (phase > 1)? 0 : phase;
    }
}

void VirtualAnalogOscillator::setFrequency(float f) {
    // TODO: linear interpolation between old and new value
    if (f < 0) {
        // TODO: error handling
        return;
    }

    frequency.setValue(f);
}