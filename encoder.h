

#ifndef STM32_MONOSYNTH_ENCODER_H
#define STM32_MONOSYNTH_ENCODER_H

#include "miosix.h"

class Encoder {
public:

    Encoder(TIM_TypeDef* timer, GPIO_TypeDef* gpio, uint32_t pin1, uint32_t pin2);


    /**
     * Getter for value attribute.
     *
     * @return  value
     */
    float getValue();

private:
    TIM_TypeDef *timer;

    /**
     * Value of the encoder, between 0 and 1.
     */
    float value;

    /**
     * Value from the last getValue() call.
     */
    float lastValue;

};

#endif //STM32_MONOSYNTH_ENCODER_H
