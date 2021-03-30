#include "miosix.h"
#include "mono_synth.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../drivers/stm32f407vg_discovery/audio.h"


void MonoSynth::process() {
    virtualAnalogOscillator.process(oscillatorBuffer);
    amplifierEnvelope.process(amplifierEnvelopeBuffer);
    oscillatorBuffer.multiply(amplifierEnvelopeBuffer);
    getBuffer().copyOnChannel(oscillatorBuffer, 0);
    getBuffer().copyOnChannel(oscillatorBuffer, 1);

}

void MonoSynth::setFrequency(float f) {
//    miosix::FastInterruptDisableLock lock;
//    miosix::FastMutex mutex;
    virtualAnalogOscillator.setFrequency(f);
}

void MonoSynth::setGlide(float glideTime) {
//    miosix::FastInterruptDisableLock lock;
//    miosix::FastMutex mutex;
    virtualAnalogOscillator.setGlide(glideTime, getSampleRate());
}

void MonoSynth::noteOn(Midi::Note midiNote) {
    // TODO: handle velocity
    triggerEnvelopeOn();
    setFrequency(Midi::midi2freq(midiNote.getMidiNote()));
}

void MonoSynth::noteOff(Midi::Note midiNote) {
    triggerEnvelopeOff();
}

void MonoSynth::triggerEnvelopeOn() {
//    miosix::FastInterruptDisableLock lock;
    amplifierEnvelope.triggerOn();
}

void MonoSynth::triggerEnvelopeOff() {
    amplifierEnvelope.triggerOff();
}