
#ifndef STM32_MONOSYNTH_DOUBLEENCODER_H
#define STM32_MONOSYNTH_DOUBLEENCODER_H

#include "encoder.h"
#include "button.h"

/**
 * A DoubleEncoder is an encoder used in conjunction
 * with a button, storing two possible values, one for the
 * unpressed state, and one for the pressed state. The double
 * encoder is in the pressed state when the button is pressed,
 * otherwise, it's in the unpressed state.
 * This class is designed for the stm32f4xx family.
 * It's designed to be used in a thread that calls
 * its update method. The status of a button must be checked
 * in the same thread after the update.
 */
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

    virtual void update() override;

    /**
     * Getter for the button attribute. It allows to obtain
     * a reference that can be used to handle the button indipendently.
     *
     * @return button attribute
     */
    inline Button &getButton() { return button; };

    /**
     * Getter for the pressedValue attribute. It returns the value
     * of the DoubleEncoder relative to its pressed state.
     *
     * @return value of the pressed state
     */
    inline float getPressedValue() const { return pressedValue; };

    /**
     * Setter for the pressed state value
     *
     * @param newValue value set for the pressedValue attribute
     */
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

    /**
     * Button linked to the encoder state.
     * If this button is pressed the update method vary the
     * value of the pressedValue attribute.
     */
    Button button;

    /**
     * Value of the pressed state.
     */
    float pressedValue;
};


#endif //STM32_MONOSYNTH_DOUBLEENCODER_H
