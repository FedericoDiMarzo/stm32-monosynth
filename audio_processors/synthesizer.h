
#ifndef STM32_MONOSYNTH_SYNTHETIZER_H
#define STM32_MONOSYNTH_SYNTHETIZER_H

#include "../audio/audio_processor.h"
#include "../midi/midi.h"

/**
 * Abstract class defining the methods used by a
 * synthesizer audio processor.
 */
class Synthesizer : public AudioProcessor {
public:

    /**
     * Method for handling input midi messages.
     * This abstract class implements just the calls of the methods
     * noteOn and noteOff.
     *
     * @param midiMessage input midi message
     */
    void processMidi(Midi::MidiMessage midiMessage);

protected:

    /**
     * Handler for the NOTE_ON midi message.
     *
     * @param note midi note related to the NOTE_ON message
     */
    virtual void noteOn(Midi::Note note) = 0;


    /**
     * Handler for the NOTE_ON midi message.
     *
     * @param note midi note related to the NOTE_ON message
     */
    virtual void noteOff(Midi::Note note) = 0;

};


#endif //STM32_MONOSYNTH_SYNTHETIZER_H
