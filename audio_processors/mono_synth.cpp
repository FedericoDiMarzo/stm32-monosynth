#include "miosix.h"
#include "mono_synth.h"
#include "../audio/audio_buffer.hpp"
#include "../audio/audio_module.h"
#include "../audio/midi.h"


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

void MonoSynth::setGlide(float glideTime) {
    miosix::FastInterruptDisableLock lock;
    virtualAnalogOscillator.setGlide(glideTime, getSampleRate());
}

void MonoSynth::setNote(uint8_t midiNote) {
    setFrequency(Midi::midi2freq(midiNote));
}