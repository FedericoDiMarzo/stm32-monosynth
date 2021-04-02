#include "synthesizer.h"

void Synthesizer::processMidi(Midi::MidiMessage midiMessage) {
    Midi::MidiMessageType type = midiMessage.getType();
    if (type == Midi::MidiMessageType::NOTE_ON) {
        noteOn(midiMessage.getNote());
    } else if (type == Midi::MidiMessageType::NOTE_OFF) {
        noteOff(midiMessage.getNote());
    }
}
