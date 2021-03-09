
#include "core_util.h"
#include "miosix.h"
#include <map>
#include <array>


void CoreUtil::rccEnableGpio(GPIO_TypeDef *gpio) {
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
        // TODO: error handling
    }
}

void CoreUtil::rccEnableTimer(TIM_TypeDef *timer) {
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
        // TODO: error handling
    }
}


uint32_t CoreUtil::gpioGetTimerAF(GPIO_TypeDef *gpio, uint32_t pin) {
    // TODO: test all states
    uint32_t af = INVALID_AF;
    if (gpio == GPIOA) {
        if (pin == 0) { // TIM5 CH1
            af = 4;
        } else if (pin == 1) { // TIM5 CH2
            af = 4;
        } else if (pin == 2) { // TIM9 CH1
            af = 2;
        } else if (pin == 3) { // TIM9 CH2
            af = 2;
        } else if (pin == 8) { // TIM1 CH1
            af = 2;
        } else if (pin == 9) { // TIM1 CH2
            af = 1;
        }
    } else if (gpio == GPIOB) {
        if (pin == 4) { // TIM3 CH1
            af = 2;
        } else if (pin == 5) { // TIM3 CH2
            af = 4;
        } else if (pin == 6) { // TIM4 CH1
            af = 1;
        } else if (pin == 7) { // TIM3 CH2
            af = 4;
        } else if (pin == 14) { // TIM12 CH1
            af = 2;
        } else if (pin == 15) { // TIM12 CH2
            af = 4;
        }
    } else if (gpio == GPIOC) {
        if (pin == 6) { // TIM8 CH1
            af = 1;
        } else if (pin == 7) { // TIM8 CH2
            af = 1;
        }
    } else if (gpio == GPIOD) {
        if (pin == 12) { // TIM4 CH1
            af = 2;
        } else if (pin == 13) { // TIM4 CH2
            af = 2;
        }
    }

    // TODO: complete the list
    return af;
}


