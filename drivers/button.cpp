
#include "button.h"
#include "miosix.h"
#include "core_util.h"

static std::function<void(void)> emptyCallback = []() {};

Button::Button(GPIO_TypeDef *gpio, uint8_t pin) :
        gpio(gpio),
        gpioPin(pin),
        previousState(false),
        callback(emptyCallback),
        triggeringType(ButtonTriggeringType::RISING) {

    {
        miosix::FastInterruptDisableLock dLock;

// enabling clock
//        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enabling SYSCFGEN peripheral
        CoreUtil::rccEnableGpio(gpio);

// GPIO configuration
        gpio->MODER &= ~(3 << (gpioPin * 2)); // setting the GPIO in input mode
        gpio->PUPDR &= ~(3 << (gpioPin * 2));
        gpio->OTYPER &= ~(1 << gpioPin);
        gpio->PUPDR |= (2 << (gpioPin * 2)); // pull-down

//        // EXTI configuration
//        EXTI->EMR |= (1 << gpioPin); // enabling the EXTI event channel for the pin
//        EXTI->RTSR &= ~(1 << gpioPin); // resetting the rising trigger register
//        EXTI->FTSR &= ~(1 << gpioPin); // resetting the falling trigger register
//
//        uint8_t portNumber;
//        if (gpio == GPIOA) {
//            portNumber = 0;
//        } else if (gpio == GPIOB) {
//            portNumber = 1;
//        } else if (gpio == GPIOC) {
//            portNumber = 2;
//        } else if (gpio == GPIOD) {
//            portNumber = 3;
//        } else if (gpio == GPIOE) {
//            portNumber = 4;
//        } else if (gpio == GPIOF) {
//            portNumber = 5;
//        } else if (gpio == GPIOG) {
//            portNumber = 6;
//        } else if (gpio == GPIOH) {
//            portNumber = 7;
//        } else if (gpio == GPIOI) {
//            portNumber = 8;
//        }
//
//        // selecting the port for EXTI channel
//        SYSCFG->EXTICR[portNumber] &= ~(0xf << (gpioPin % 4));
//        SYSCFG->EXTICR[portNumber] |= (portNumber << (gpioPin % 4));
    }


    setTriggeringType(triggeringType);
}

void Button::setTriggeringType(ButtonTriggeringType triggering) {
    triggeringType = triggering;
    {
        miosix::FastInterruptDisableLock dLock;

        EXTI->RTSR &= ~(1 << gpioPin); // resetting the rising trigger register
        EXTI->FTSR &= ~(1 << gpioPin); // resetting the falling trigger register

        if (triggering == ButtonTriggeringType::RISING)
            EXTI->RTSR = 1 << gpioPin; // rising edge event
        else
            EXTI->FTSR = 1 << gpioPin; // falling edge event
    }
}

void Button::update() {
    bool currentState = isPressed();
    if (previousState != currentState) { // event detected
        if (triggeringType == ButtonTriggeringType::FALLING && previousState) {
            callback(); // callback on falling edge
        } else if (triggeringType == ButtonTriggeringType::RISING && !previousState) {
            callback(); // callback on rising edge
        }
    }
    previousState = currentState;
}

bool Button::isPressed() {
    return gpio->IDR & (1 << gpioPin);
}