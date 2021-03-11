#include "miosix.h"
#include "mono_synth.h"
#include "../audio/audio_buffer.hpp"
#include "../audio/audio_module.h"


MonoSynth::MonoSynth()
        : virtualAnalogOscillator(*this) {
    oscillatorBuffer.clear();
}

void MonoSynth::process() {
    virtualAnalogOscillator.process(oscillatorBuffer);
    getBuffer().copyOnChannel(oscillatorBuffer, 0);
    getBuffer().copyOnChannel(oscillatorBuffer, 1);
//    getBuffer().clear();
}

void MonoSynth::setFrequency(float f) {
    miosix::FastInterruptDisableLock lock;
    virtualAnalogOscillator.setFrequency(f);
}