

#ifndef STM32_MONOSYNTH_MONO_SYNTH_H
#define STM32_MONOSYNTH_MONO_SYNTH_H

#include "../drivers/stm32f407vg_discovery/audio.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_buffer.h"
#include "../audio_modules/virtual_analog_oscillator.h"
#include "../audio_modules/envelope.h"

class MonoSynth : public AudioProcessor {
public:
    MonoSynth();

    void process() override;

    void setFrequency(float f);

    void setGlide(float glideTime);

    void setNote(uint8_t midiNote);

    void triggerEnvelopeOn();

    void triggerEnvelopeOff();

private:
    VirtualAnalogOscillator virtualAnalogOscillator;

    Envelope amplifierEnvelope;

    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> oscillatorBuffer;

    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> amplifierEnvelopeBuffer;
};

#endif //STM32_MONOSYNTH_MONO_SYNTH_H
