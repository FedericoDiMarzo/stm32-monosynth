

#ifndef STM32_MONOSYNTH_AUDIO_MODULE_H
#define STM32_MONOSYNTH_AUDIO_MODULE_H

#include "audio.h"
#include "audio_buffer.h"

template<size_t CHANNEL_NUM>
class AudioModule {
public:

    /**
     * Constructor.
     */
    AudioModule() {};

    /**
     * Precess an external AudioBuffer.
     *
     * An implementation of an AudioModule must implement this
     * virtual method.
     *
     * @param buffer AudioBuffer to be processed
     */
    virtual void process(AudioBuffer<float, CHANNEL_NUM, AUDIO_DRIVER_BUFFER_SIZE> &buffer) = 0;


private:
    /**
    * Disabling copy constructor.
    */
    AudioModule<CHANNEL_NUM>(const AudioModule<CHANNEL_NUM> &);

    /**
     * Disabling move operator.
     */
    AudioModule<CHANNEL_NUM> &operator=(AudioModule<CHANNEL_NUM> &);
};

#endif //STM32_MONOSYNTH_AUDIO_MODULE_H
