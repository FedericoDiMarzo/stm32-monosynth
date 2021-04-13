

#ifndef STM32_MONOSYNTH_MONO_SYNTH_H
#define STM32_MONOSYNTH_MONO_SYNTH_H

#include "../drivers/common/audio.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_buffer.h"
#include "../audio_modules/virtual_analog_oscillator.h"
#include "../audio_modules/lowpass_filter_1p.h"
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
     *
     * @param audioDriver
     */
    MonoSynth(AudioDriver &audioDriver) :
            Synthesizer(audioDriver),
            lowpassFilter(*this),
            oscillator(*this),
            amplifierEnvelope(*this),
            normalizedVelocity(0) {};

    void process() override;

    /**
     * Gets the lowpass filter.
     *
     * @return lowpass filter audio module
     */
    inline LowpassFilter1P &getLowpassFilter() { return lowpassFilter; };

    /**
     * Gets the amplifier envelope
     *
     * @return envelope audio module
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
     * Low pass filter.
     */
    LowpassFilter1P lowpassFilter;

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
