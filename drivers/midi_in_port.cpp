#include "miosix.h"
#include "midi_in_port.h"
#include "hardware_configuration.h"
#include <queue>

MidiInPort &MidiInPort::getInstance() {
    static MidiInPort singleton;
    return singleton;
}

void MidiInPort::fillTheQueue(size_t maxNotesRead) {
    uint8_t buffer[3]; // buffer containing a single note on/off message
    size_t counter = 0;
    // cycle until maxNotesRead messages are read
    // or a read error is encountered
    while (serialPort.readBlock(buffer, 3, 0) != -1) {
        // note message found
        bool isStatusByte = buffer[0] & 0x80;
        bool isNoteMessage = isStatusByte && (((buffer[0] & 0x70) >> 4) <= 1);
        if (isNoteMessage || !isStatusByte) {
            rawMidiQueue.push(buffer[0]); // status byte
            rawMidiQueue.push(buffer[1]); // note
            rawMidiQueue.push(buffer[2]); // velocity
            counter++;
        }

        // all maxNotesRead messages read
        if (counter >= maxNotesRead) break;
    }
}