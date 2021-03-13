

#ifndef STM32_MONOSYNTH_MONO_SYNTH_H
#define STM32_MONOSYNTH_MONO_SYNTH_H

#include "../audio/audio.h"
#include "../audio/audio_processor.h"
#include "../audio/audio_buffer.h"
#include "../audio_modules/virtual_analog_oscillator.h"

class MonoSynth : public AudioProcessor {
public:
    MonoSynth();

    void process() override;

    void setFrequency(float f);

    void setGlide(float glideTime);

    void setNote(uint8_t midiNote);

private:
    VirtualAnalogOscillator virtualAnalogOscillator;

    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> oscillatorBuffer;
};

#endif //STM32_MONOSYNTH_MONO_SYNTH_H
