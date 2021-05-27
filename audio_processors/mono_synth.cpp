#include "miosix.h"
#include "mono_synth.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../drivers/common/audio.h"


void MonoSynth::process() {
    oscillator.process(oscillatorBuffer);
    lowpassFilter.process(oscillatorBuffer);
    envelope.process(envelopeBuffer);
    oscillatorBuffer.multiply(envelopeBuffer);
    oscillatorBuffer.applyGain(normalizedVelocity);
    getBuffer().copyOnChannel(oscillatorBuffer, 0);
    getBuffer().copyOnChannel(oscillatorBuffer, 1);

}

void MonoSynth::setOscillatorFrequency(float f) {
    if (f < 0.0f) return;
    oscillator.setFrequency(f);
}

void MonoSynth::setGlide(float glideTime) {
    oscillator.setGlide(glideTime, getSampleRate());
}

void MonoSynth::setFilterCutoff(float normalizedValue) {
    baseCutoffNormalizedFrequency = AudioMath::clip(normalizedValue, 0.0f, 1.0f);
    lowpassFilter.setCutoff(getKeytrackCutoff(lastMidiNote));
}

void MonoSynth::setFilterKeyTracking(float normalizedValue) {
    normalizedValue = AudioMath::clip(normalizedValue, 0.0f, 1.0f);
    filterKeyTracking = normalizedValue;
}

float MonoSynth::getKeytrackCutoff(uint8_t midiNote) {
    float noteScaling = (static_cast<float>(midiNote) / 127.0f);
    return baseCutoffNormalizedFrequency * (1 + (noteScaling - 1) * filterKeyTracking);
}

void MonoSynth::noteOn(Midi::Note note) {
    // TODO: test velocity
    lastMidiNote = note.getMidiNote();
    setOscillatorFrequency(Midi::midi2freq(lastMidiNote));

    // velocity
    // TODO: velocity compression
    normalizedVelocity = AudioMath::linearMap(static_cast<float>(note.getVelocity()),
                                              0.0f, 127.0f, 0.5f, 1.0f);

    // filter keytracking
    lowpassFilter.setCutoff(getKeytrackCutoff(lastMidiNote));

    triggerEnvelopeOn();
}

void MonoSynth::noteOff(Midi::Note note) {
    triggerEnvelopeOff();
}

void MonoSynth::triggerEnvelopeOn() {
    envelope.triggerOn();
}

void MonoSynth::triggerEnvelopeOff() {
    envelope.triggerOff();
}



