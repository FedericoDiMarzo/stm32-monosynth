

#ifndef STM32_MONOSYNTH_MONO_SYNTH_H
#define STM32_MONOSYNTH_MONO_SYNTH_H

#include "../drivers/common/audio.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_buffer.h"
#include "../audio_modules/virtual_analog_oscillator.h"
#include "../audio_modules/lowpass_filter_1p.h"
#include "../audio_modules/lowpass_filter_ladder_4p.h"
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
            oscillator(*this),
            lowpassFilter(*this),
            envelope(*this),
            normalizedVelocity(1.0f),
            baseCutoffNormalizedFrequency(1.0f),
            lastMidiNote(50),
            filterKeyTracking(0.5f) {

        // modulating the filter with the envelope
        lowpassFilter.setCutoffModulatorPtr(envelopeBuffer.getReadPointer(0));
    };

    void process() override;

    /**
     * Gets the lowpass filter.
     *
     * @return lowpass filter audio module
     */
    inline LowpassFilterLadder4p &getLowpassFilter() { return lowpassFilter; };

    /**
     * Gets the envelope
     *
     * @return envelope audio module
     */
    inline Envelope &getEnvelope() { return envelope; };

    /**
     * Sets the cutoff of the lowpass filter considering
     * the keytracking.
     *
     * @param normalizedValue cutoff value between 0 and 1
     */
    void setFilterCutoff(float normalizedValue);

    /**
     * Sets the resonance of the lowpass filter.
     *
     * @param normalizedValue resonance value between 0 and 1
     */
    inline void setFilterResonance(float normalizedValue) { lowpassFilter.setResonance(normalizedValue); };

    /**
     * Sets the glide time between the notes.
     *
     * @param glideTime time in seconds
     */
    void setGlide(float glideTime);

    /**
     * Sets the keytracking amount for the filter cutoff.
     *
     * @param normalizedValue value between 0 and 1
     */
    void setFilterKeyTracking(float normalizedValue);

    /**
     * Sets the envelope amount for the filter cutoff.
     *
     * @param normalizedValue value between 0 and 1
     */
    inline void setFilterCutoffEnvelopeAmt(float normalizedValue) {
        lowpassFilter.setCutoffModulationAmount(normalizedValue);
    };


private:

    /**
     * Oscillator audio module.
     */
    VirtualAnalogOscillator oscillator;

    /**
     * Low pass filter.
     */
    LowpassFilterLadder4p lowpassFilter;

    /**
     * Envelope for the output amplifier and for the filter cutoff.
     */
    Envelope envelope;

    /**
     * Gain multiplier based on the
     * velocity of a noteOn.
     */
    float normalizedVelocity;

    /**
     * Base cutoff frequency in a normalized range (between 0 and 1)
     * used as a base value before the filter keytracking.
     */
    float baseCutoffNormalizedFrequency;

    /**
     * Last midi note value played.
     */
    uint8_t lastMidiNote;

    /**
     * Intensity of the note tracking for
     * the filter cutoff. Values between 0 and 1.
     */
    float filterKeyTracking;

    /**
     * AudioBuffer used to render the VA oscillator.
     */
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> oscillatorBuffer;

    /**
     * AudioBuffer used to render the envelope.
     */
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> envelopeBuffer;

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
    void setOscillatorFrequency(float f);

    void noteOn(Midi::Note note) override;

    void noteOff(Midi::Note note) override;

    /**
     * Gets the normalized value (between 0 and 1) of the
     * filter cutoff after the keytrack is applied.
     * @return
     */
    float getKeytrackCutoff(uint8_t midiNote);

};

#endif //STM32_MONOSYNTH_MONO_SYNTH_H
