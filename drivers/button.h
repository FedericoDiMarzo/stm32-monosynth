
#ifndef STM32_MONOSYNTH_BUTTON_H
#define STM32_MONOSYNTH_BUTTON_H

#include "miosix.h"
#include <functional>

/**
 * Class to handle an hardware button in an stm32f4xx board.
 * It's designed to be used in a thread that does
 * update the status of the it.
 * The status of a button must be checked
 * in the same thread.
 */
class Button {
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

    /**
     * Sets a callback function that's triggered when
     * the button has been released.
     *
     * @param newCallback functor callback
     */
    inline void setCallbackButtonReleased(std::function<void(void)> newCallback) { callbackButtonReleased = newCallback; };

    /**
     * Sets a callback function that's triggered when
     * the button has been pressed.
     *
     * @param newCallback functor callback
     */
    inline void setCallbackButtonPressed(std::function<void(void)> newCallback) { callbackButtonPressed = newCallback; };

    /**
     * This function updates the state of the button.
     * It's suggested to call this function in a loop of a
     * dedicated thread handling the hardware input interface.
     * This function checks the input state of the GPIO pin, updates
     * the internal state of the Button and calls the callbacks
     * in case of a button press or release event.
     */
    void update();

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

    /**
     * Callback for a button release event.
     */
    std::function<void(void)> callbackButtonReleased;

    /**
     * Callback for a button press event.
     */
    std::function<void(void)> callbackButtonPressed;
};


#endif //STM32_MONOSYNTH_BUTTON_H
