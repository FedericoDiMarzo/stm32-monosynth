#include "miosix.h"
#include "mono_synth.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_module.h"
#include "../drivers/stm32f407vg_discovery/audio.h"


void MonoSynth::process() {
//    oscillatorBuffer.clear();
//    for (auto vaOsc : virtualAnalogOscillators) {
//        vaOsc.process(oscillatorTmpBuffer);
//        oscillatorBuffer.add(oscillatorTmpBuffer);
//    }
//    oscillatorBuffer.applyGain(1 / static_cast<float>(virtualAnalogOscillators.size()));
    oscillator.process(oscillatorBuffer);
    amplifierEnvelope.process(amplifierEnvelopeBuffer);
    oscillatorBuffer.multiply(amplifierEnvelopeBuffer);
    oscillatorBuffer.applyGain(0.3);
    getBuffer().copyOnChannel(oscillatorBuffer, 0);
    getBuffer().copyOnChannel(oscillatorBuffer, 1);

}

void MonoSynth::setFrequency(float f) {
    if (f < 0) return;
    oscillator.setFrequency(f);
//    for (unsigned int i = 0; i < virtualAnalogOscillators.size(); i++) {
////        virtualAnalogOscillators[i].setFrequency(f * (1 + getVirtualAnalogVoiceDetune(i)));
//        virtualAnalogOscillators[i].setFrequency(f);
//}
}

void MonoSynth::setDetune(float d) {
    // clipping
    d = (d < 0) ? 0 : d;
    d = (d > 1) ? 1 : d;

    // remapping d before assigning
    detune = AudioMath::linearMap(d, 0, 1, 0, 0.3);
}

void MonoSynth::setGlide(float glideTime) {
//    for (auto vaOsc : virtualAnalogOscillators) {
//        vaOsc.setGlide(glideTime, getSampleRate());
//    }
    oscillator.setGlide(glideTime, getSampleRate());
}

void MonoSynth::noteOn(Midi::Note note) {
    // TODO: handle velocity
    setFrequency(Midi::midi2freq(note.getMidiNote()));
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

float MonoSynth::getVirtualAnalogVoiceDetune(size_t index) {
//    if (index < 0 || index > virtualAnalogOscillators.size()) return 0;
//
//    float halfOscillatorCount = static_cast<float>(virtualAnalogOscillators.size()) / 2.0f;
//    return (detune / halfOscillatorCount * (index - halfOscillatorCount));
    return 0;
}

