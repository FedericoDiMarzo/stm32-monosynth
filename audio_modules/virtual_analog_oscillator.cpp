
#include "../audio/audio.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "virtual_analog_oscillator.h"

VirtualAnalogOscillator::VirtualAnalogOscillator(float sampleRate)
        :
        sampleRate(sampleRate),
        frequency(2000.0),
        phase(0),
        lastSample(0.0) {

}

AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &VirtualAnalogOscillator::process() {
    for (uint32_t i; i < buffer.getBufferLength(); i++) {

    }

    return buffer;
}