

#ifndef STM32_MONOSYNTH_MONO_SYNTH_H
#define STM32_MONOSYNTH_MONO_SYNTH_H

#include "../drivers/stm32f407vg_discovery/audio.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_buffer.h"
#include "../audio_modules/virtual_analog_oscillator.h"
#include "../audio_modules/envelope.h"
#include "../midi/midi.h"

/**
 * Audio processor implementing the monophonic synthesizer.
 */
class MonoSynth : public AudioProcessor {
public:
    /**
     * Constructor.
     */
    MonoSynth() :
            virtualAnalogOscillator(*this),
            amplifierEnvelope(*this) {};

    void process() override;

    /**
     * Getter for amplifierEnvelope attribute.
     *
     * @return amplifierEnvelope
     */
    inline Envelope &getAmplifierEnvelope() { return amplifierEnvelope; };

    /**
     * Sets the frequency played by the synth.
     *
     * @param f note frequency
     */
    void setFrequency(float f);

    /**
     * Sets the glide time between the notes.
     *
     * @param glideTime time in seconds
     */
    void setGlide(float glideTime);

    /**
     * Sets the midi note played by the synth.
     *
     * @param midiNote value of the midi note
     */
    void noteOn(Midi::Note midiNote);

    void noteOff(Midi::Note midiNote);

private:
    /**
     * Oscillator implementing different VA algorithms.
     */
    VirtualAnalogOscillator virtualAnalogOscillator;

    /**
     * Envelope for the output amplifier.
     */
    Envelope amplifierEnvelope;

    /**
     * AudioBuffer used to render the VA oscillator.
     */
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> oscillatorBuffer;

    /**
     * AudioBuffer used to render the amplifier envelope.
     */
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> amplifierEnvelopeBuffer;

    /**
    * Triggers the amplitude envelope.
    */
    void triggerEnvelopeOn();

    /**
     * Release the amplitude envelope trigger.
     */
    void triggerEnvelopeOff();

};

#endif //STM32_MONOSYNTH_MONO_SYNTH_H
