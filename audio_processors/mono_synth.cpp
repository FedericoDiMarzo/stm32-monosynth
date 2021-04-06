#include "miosix.h"
#include "mono_synth.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../drivers/stm32f407vg_discovery/audio.h"


void MonoSynth::process() {
    oscillator.process(oscillatorBuffer);
    amplifierEnvelope.process(amplifierEnvelopeBuffer);
    oscillatorBuffer.multiply(amplifierEnvelopeBuffer);
    oscillatorBuffer.applyGain(normalizedVelocity);
    getBuffer().copyOnChannel(oscillatorBuffer, 0);
    getBuffer().copyOnChannel(oscillatorBuffer, 1);

}

void MonoSynth::setFrequency(float f) {
    if (f < 0.0f) return;
    oscillator.setFrequency(f);
}


void MonoSynth::setGlide(float glideTime) {
    oscillator.setGlide(glideTime, getSampleRate());
}

void MonoSynth::noteOn(Midi::Note note) {
    // TODO: test velocity
    setFrequency(Midi::midi2freq(note.getMidiNote()));
    normalizedVelocity = AudioMath::linearMap(static_cast<float>(note.getVelocity()),
            0.0f, 127.0f, 0.0f, 1.0f);
    triggerEnvelopeOn();
}

void MonoSynth::noteOff(Midi::Note note) {
    triggerEnvelopeOff();
}

void MonoSynth::triggerEnvelopeOn() {
    amplifierEnvelope.triggerOn();
}

void MonoSynth::triggerEnvelopeOff() {
    amplifierEnvelope.triggerOff();
}


