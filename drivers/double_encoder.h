
#ifndef STM32_MONOSYNTH_DOUBLEENCODER_H
#define STM32_MONOSYNTH_DOUBLEENCODER_H

#include "encoder.h"
#include "button.h"

class DoubleEncoder : public Encoder {
public:
    /**
     * Constructor, it initialize an encoder and a button.
     *
     * @param timer timer used in encoder mode
     * @param encoderGpio encoder input GPIO
     * @param encoderPin1 first encoder pulse output
     * @param encoderPin2 second encoder pulse output
     */
    DoubleEncoder(TIM_TypeDef *timer,
                  GPIO_TypeDef *encoderGpio,
                  uint8_t encoderPin1,
                  uint8_t encoderPin2,
                  GPIO_TypeDef *buttonGpio,
                  uint8_t buttonPin);

    void update() override;

    inline float getPressedValue() const { return pressedValue; };

    void setPressedValue(float newValue);

    /**
     * Disabling default constructor.
     */
    DoubleEncoder() = delete;

    /**
     * Disabling copy constructor.
     */
    DoubleEncoder(DoubleEncoder &) = delete;

    /**
     * Disabling move operator.
     */
    DoubleEncoder &operator=(DoubleEncoder &) = delete;

private:
    typedef Encoder P; // parent type
    Button button;
    float pressedValue;
};


#endif //STM32_MONOSYNTH_DOUBLEENCODER_H
