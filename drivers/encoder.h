

#ifndef STM32_MONOSYNTH_ENCODER_H
#define STM32_MONOSYNTH_ENCODER_H

#include "miosix.h"

#define SENSITIVITY_OFFSET (1500.0f)

class Encoder {
public:

    Encoder(TIM_TypeDef *timer, GPIO_TypeDef *gpio, uint8_t pin1, uint8_t pin2);


    /**
     * Getter for value attribute.
     *
     * @return  value
     */
    float getValue();

    inline float getSensitivity() { return sensitivity; };

    inline void setSensitivity(float newValue) { sensitivity = newValue; };

private:
    TIM_TypeDef *timer;

    /**
     * Value of the encoder, between 0 and 1.
     */
    float value;

    const uint32_t arrValue = 0xFFFF - 1;

    float sensitivity;

};

#endif //STM32_MONOSYNTH_ENCODER_H
