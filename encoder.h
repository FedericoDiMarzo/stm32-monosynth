

#ifndef STM32_MONOSYNTH_ENCODER_H
#define STM32_MONOSYNTH_ENCODER_H

#include "miosix.h"


class Encoder {
public:

    Encoder(GPIO_TypeDef *gpio = GPIOB, unsigned int pin1 = 4, unsigned int pin2 = 5,
            TIM_TypeDef *timer = TIM3);

    static void testStaticRotary();

    /**
     * Getter for value attribute.
     *
     * @return  value
     */
    float getValue();

private:
    TIM_TypeDef *_timer;

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
