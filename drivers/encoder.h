

#ifndef STM32_MONOSYNTH_ENCODER_H
#define STM32_MONOSYNTH_ENCODER_H

#include "miosix.h"
#include "hardware_updatable.h"

#define ENCODER_SENSITIVITY_OFFSET (1500.0f)

/**
 * Class to handle an hardware encoder in a stm32f4xx board.
 * The class uses the timer of the board in encoder mode and
 * must be initialized with a timer that supports that function.
 * The encoder should send rising edge pulses, thus the common
 * pin must be connected to a 3V voltage source.
 * It's designed to be used in a thread that calls
 * its update method. The status of a button must be checked
 * in the same thread after the update.
 */
class Encoder : public HardwareUpdatable {
public:

    /**
     * Constructor, it initializes the timer in encoder mode and the
     * GPIO pins in pull-down mode with the appropriate alternate function.
     *
     * @param timer timer used in encoder mode
     * @param gpio input GPIO
     * @param pin1 first encoder pulse output
     * @param pin2 second encoder pulse output
     */
    Encoder(TIM_TypeDef *timer, GPIO_TypeDef *gpio, uint8_t pin1, uint8_t pin2);

    void update() override;

    /**
     * Getter for value attribute.
     *
     * @return value between 0 and 1
     */
    inline float getValue() const { return value; };

    /**
     * Sets the value of the encoder between 0 and 1.
     * Values outside this range will be clipped.
     *
     * @param newValue new encoder value
     */
    void setValue(float newValue);

    /**
     * Getter for sensitivity attribute.
     *
     * @return sensitivity
     */
    inline float getSensitivity() const { return sensitivity; };

    /**
     * Setter for sensitivity parameter.
     * Higher values of the sensitivity let the value
     * of the encoder change with less rotation.
     *
     * @param newValue new sensitivity value
     */
    inline void setSensitivity(float newValue) { sensitivity = newValue; };

    /**
     * Disabling default constructor.
     */
    Encoder() = delete;

    /**
     * Disabling copy constructor.
     */
    Encoder(Encoder &) = delete;

    /**
     * Disabling move operator.
     */
    Encoder &operator=(Encoder &) = delete;

protected:
    /**
     * Utility function that returns the increment
     * of the encoder before the last call.
     *
     * @return encoder increment
     */
    float getIncrement() const;

    /**
     * Timer pointer.
     */
    TIM_TypeDef *timer;

    /**
     * Value of the encoder, between 0 and 1.
     */
    float value;

    /**
     * Rotation sensitivity.
     * Change this attribute to scale the
     * return value of the getIncrement method.
     */
    float sensitivity;

    /**
     * Auto reload register value.
     */
    const uint32_t arrValue = 0xFFFF - 1;


};

#endif //STM32_MONOSYNTH_ENCODER_H
