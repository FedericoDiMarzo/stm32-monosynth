
#ifndef STM32_MONOSYNTH_BUTTON_H
#define STM32_MONOSYNTH_BUTTON_H

#include "miosix.h"
#include "hardware_updatable.h"
#include <functional>

/**
 * Class to handle an hardware button in an stm32f4xx board.
 * It's designed to be used in a thread that calls
 * its update method.
 * The status of a button must be checked
 * in the same thread after the update.
 */
class Button : HardwareUpdatable {
public:
    /**
     * Constructor.
     *
     * @param gpio input port
     * @param pin input pin
     */
    Button(GPIO_TypeDef *gpio, uint8_t pin);

    /**
     * Returns true if the button is pressed.
     *
     * @return button state
     */
    bool isPressed();

    /**
     * Returns true if the button has been pressed;
     * equivalent to a rising edge on the pin.
     *
     * @return true if the button has been pressed@
     */
    inline bool switchedOn() { return (isPressed() != previousState) && !previousState; };

    /**
     * Returns true if the button has been released;
     * equivalent to a falling edge on the pin.
     *
     * @return true if the button has been released
     */
    inline bool switchedOff() { return (isPressed() != previousState) && previousState; };

    void update();

    /**
    * Deleting default constructor.
    */
    Button() = delete;

    /**
    * Deleting copy constructor.
    */
    Button(Button &) = delete;

    /**
     * Deleting move operator.
     */
    Button &operator=(Button &) = delete;

private:
    /**
     * Pointer to the GPIO.
     */
    GPIO_TypeDef *gpio;

    /**
     * Pin of the GPIO.
     */
    uint8_t gpioPin;

    /**
     * Saves the state of the button (HIGH/LOW)
     * of the last state update.
     */
    bool previousState;

};


#endif //STM32_MONOSYNTH_BUTTON_H
