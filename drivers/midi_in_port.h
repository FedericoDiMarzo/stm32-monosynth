#ifndef STM32_MONOSYNTH_MIDI_IN_PORT_H
#define STM32_MONOSYNTH_MIDI_IN_PORT_H

#include "miosix.h"
#include "../miosix/arch/common/drivers/serial.h"
#include "../midi/midi.h"
#include "hardware_configuration.h"
#include <queue>

// TODO: find the right value
#define MIDI_IN_BAUDRATE 3000

/**
 * Singleton class that implements a midi receiver
 * through the UART protocol.
 * The USART interface and the pins can be configured
 * in the "hardware_configuration.h" header.
 */
class MidiInPort {
public:

    /**
     * Gets the singleton instance.
     *
     * @return MidiIn singleton
     */
    static MidiInPort &getInstance();


    /**
     * Gets the raw midi queue
     *
     * @return rawMidiQueue
     */
    inline std::queue <uint8_t> &getRawMidiQueue() { return rawMidiQueue; };

    /**
     * Fills the read queue with incoming midi messages from the UART port.
     * Midi messages different from a note on/off are filtered out.
     *
     * @param maxNotesRead maximum amount of note on/off messages inserted in the queue
     */
    void fillTheQueue(size_t maxNotesRead);

    /**
     * Copy constructor disabled.
     */
    MidiInPort(MidiInPort &) = delete;

    /**
     * Move operator disabled.
     */
    MidiInPort &operator=(MidiInPort &) = delete;


private:
    /**
     * Queue containing raw midi data.
     */
    std::queue <uint8_t> rawMidiQueue;

    /**
     * Serial port driver.
     * It's initialized with the midi baudrate.
     */
    miosix::STM32Serial serialPort;

    /**
     * Private constructor.
     */
    MidiInPort() : serialPort(MidiInUsartId, 31250) {}
};


#endif //STM32_MONOSYNTH_MIDI_IN_PORT_H
