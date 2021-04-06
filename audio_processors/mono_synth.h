

#ifndef STM32_MONOSYNTH_MONO_SYNTH_H
#define STM32_MONOSYNTH_MONO_SYNTH_H

#include "../drivers/stm32f407vg_discovery/audio.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_buffer.h"
#include "../audio_modules/virtual_analog_oscillator.h"
#include "../audio_modules/envelope.h"
#include "synthesizer.h"
#include "../midi/midi.h"
#include <vector>

/**
 * Audio processor implementing the monophonic synthesizer.
 */
class MonoSynth : public Synthesizer {
public:
    /**
     * Constructor.
     */
    MonoSynth() :
            oscillator(*this),
            amplifierEnvelope(*this),
            normalizedVelocity(0) {};

    void process() override;

    /**
     * Getter for amplifierEnvelope attribute.
     *
     * @return amplifierEnvelope
     */
    inline Envelope &getAmplifierEnvelope() { return amplifierEnvelope; };

    /**
     * Sets the glide time between the notes.
     *
     * @param glideTime time in seconds
     */
    void setGlide(float glideTime);


private:

    /**
     * Oscillator audio module.
     */
    VirtualAnalogOscillator oscillator;

    /**
     * Envelope for the output amplifier.
     */
    Envelope amplifierEnvelope;

    /**
     * Amplifier multiplier based on the
     * velocity of a noteOn.
     */
    float normalizedVelocity;

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

    /**
     * Sets the frequency played by the synth.
     *
     * @param f note frequency
     */
    void setFrequency(float f);

    void noteOn(Midi::Note note) override;

    void noteOff(Midi::Note note) override;

};

#endif //STM32_MONOSYNTH_MONO_SYNTH_H
