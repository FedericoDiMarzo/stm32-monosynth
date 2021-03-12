
#include "core_util.h"
#include "miosix.h"
#include <map>
#include <array>


void CoreUtil::rccEnableGpio(const GPIO_TypeDef *gpio) {
    // TODO: test all states
    if (gpio == GPIOA) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    } else if (gpio == GPIOB) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    } else if (gpio == GPIOC) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    } else if (gpio == GPIOD) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    } else if (gpio == GPIOE) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    } else if (gpio == GPIOF) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    } else if (gpio == GPIOG) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    } else if (gpio == GPIOH) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
    } else if (gpio == GPIOI) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
    } else {
        errorHandler();
    }
}

void CoreUtil::rccEnableTimer(const TIM_TypeDef *timer) {
    // TODO: test all states
    if (timer == TIM1) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    } else if (timer == TIM2) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    } else if (timer == TIM3) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    } else if (timer == TIM4) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    } else if (timer == TIM5) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    } else if (timer == TIM6) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    } else if (timer == TIM7) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    } else if (timer == TIM8) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
    } else if (timer == TIM9) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    } else if (timer == TIM10) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
    } else if (timer == TIM11) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
    } else if (timer == TIM12) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
    } else if (timer == TIM13) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
    } else if (timer == TIM14) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    } else {
        errorHandler();
    }
}

void CoreUtil::rccEnableI2c(const I2C_TypeDef *i2c) {
    // TODO: test all states
    if (i2c == I2C1) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    } else if (i2c == I2C2) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    } else if (i2c == I2C3) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    } else {
        errorHandler();
    }
}


uint8_t CoreUtil::getGpioTimerAf(const GPIO_TypeDef *gpio, uint8_t pin) {
    if (pin >= 16) errorHandler();
    // TODO: test all states
    uint8_t af = INVALID_AF; // default fallback

    if (gpio == GPIOA) {
        if (pin == 0) { // TIM5 CH1
            af = 2;
        } else if (pin == 1) { // TIM5 CH2
            af = 2;
        } else if (pin == 2) { // TIM9 CH1
            af = 3;
        } else if (pin == 3) { // TIM9 CH2
            af = 3;
        } else if (pin == 6) { // TIM3 CH1
            af = 2;
        } else if (pin == 7) { // TIM3 CH2
            af = 2;
        } else if (pin == 8) { // TIM1 CH1
            af = 1;
        } else if (pin == 9) { // TIM1 CH2
            af = 1;
        }

    } else if (gpio == GPIOB) {
        if (pin == 4) { // TIM3 CH1
            af = 2;
        } else if (pin == 5) { // TIM3 CH2
            af = 2;
        } else if (pin == 6) { // TIM4 CH1
            af = 2;
        } else if (pin == 7) { // TIM4 CH2
            af = 2;
        } else if (pin == 8) { // TIM10 CH1
            af = 3;
        } else if (pin == 9) { // TIM11 CH1
            af = 3;
        }

    } else if (gpio == GPIOC) {
        if (pin == 6) { // TIM8 CH1
            af = 3;
        } else if (pin == 7) { // TIM8 CH2
            af = 3;
        } else if (pin == 8) { // TIM3 CH1
            af = 2;
        } else if (pin == 9) { // TIM3 CH2
            af = 2;
        }

    } else if (gpio == GPIOD) {
        if (pin == 12) { // TIM4 CH1
            af = 2;
        } else if (pin == 13) { // TIM4 CH2
            af = 2;
        }

    } else if (gpio == GPIOE) {
        if (pin == 5) { // TIM9 CH1
            af = 3;
        } else if (pin == 6) { // TIM9 CH2
            af = 3;
        } else if (pin == 9) { // TIM1 CH1
            af = 2;
        } else if (pin == 11) { // TIM1 CH2
            af = 2;
        }

    } else if (gpio == GPIOH) {
        if (pin == 10) { // TIM5 CH1
            af = 2;
        } else if (pin == 11) { // TIM5 CH2
            af = 2;
        }
    }

    if (af == INVALID_AF) errorHandler();
    return af;
}

uint8_t CoreUtil::getGpioI2cAf(const GPIO_TypeDef *gpio, uint8_t pin) {
    if (pin >= 16) errorHandler();
    // TODO: test all states
    bool pinHasI2c = false;
    const uint8_t af = 4; // the AF is always the same

    if (gpio == GPIOA) {
        if (pin == 8) { // I2C3
            pinHasI2c = true;
        }

    } else if (gpio == GPIOB) {
        if (pin == 6 || pin == 7 || // I2C1
            pin == 8 || pin == 9 || // I2C1
            pin == 10 || pin == 11) { // I2C2
            pinHasI2c = true;
        }

    } else if (gpio == GPIOC) {
        if (pin == 9) { // I2C3
            pinHasI2c = true;
        }

    } else if (gpio == GPIOF) {
        if (pin == 0 || pin == 1) { // I2C2
            pinHasI2c = true;
        }

    } else if (gpio == GPIOH) {
        if (pin == 4 || pin == 5 || // I2C2
            pin == 7 || pin == 8) { // I2C3
            pinHasI2c = true;
        }
    }

    if (!pinHasI2c) errorHandler();
    return af;
}

void CoreUtil::errorHandler() {
    while (true);
}


