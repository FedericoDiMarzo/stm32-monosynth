
#include "button.h"
#include "miosix.h"
#include "core_util.h"

void emptyCallback() {};

Button::Button(GPIO_TypeDef *gpio, uint8_t pin) :
        gpio(gpio),
        gpioPin(pin),
        previousState(false) {

    // disabling interrupts during peripheral configuration
    miosix::FastInterruptDisableLock dLock;

    // enabling clock
    CoreUtil::rccEnableGpio(gpio);

    // GPIO configuration
    gpio->MODER &= ~(3 << (gpioPin * 2)); // setting the GPIO in input mode
    gpio->PUPDR &= ~(3 << (gpioPin * 2)); // reset
    gpio->OTYPER &= ~(1 << gpioPin); // reset
    gpio->PUPDR |= (2 << (gpioPin * 2)); // pull-down



}

void Button::update() {
    bool currentState = isPressed();
//    if (previousState != currentState) { // event detected
//        if (previousState) {
//            callbackButtonReleased(); // callback on falling edge
//        } else {
//            callbackButtonPressed(); // callback on rising edge
//        }
//    }
    previousState = currentState;
}

bool Button::isPressed() {
    return gpio->IDR & (1 << gpioPin);
}