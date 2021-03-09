
#include "encoder.h"
#include "miosix.h"
#include "core_util.h"

using namespace miosix;

Encoder::Encoder(TIM_TypeDef *timer, GPIO_TypeDef *gpio, uint32_t pin1, uint32_t pin2) :
        timer(timer),
        value(0.0f),
        lastValue(0.0f) {

    uint32_t alternateFunPin1 = CoreUtil::gpioGetTimerAF(gpio, pin1);
    uint32_t alternateFunPin2 = CoreUtil::gpioGetTimerAF(gpio, pin2);
    if (alternateFunPin1 == INVALID_AF | alternateFunPin2 == INVALID_AF) {
        return; // TODO: error handling
    }

    int filterOrder = 8;
    int flagAFR1 = (pin1 < 8) ? 0 : 1;
    int flagAFR2 = (pin2 < 8) ? 0 : 1;

    CoreUtil::rccEnableGpio(gpio);
    CoreUtil::rccEnableTimer(timer);

    // gpio configuration
    gpio->MODER &= ~((3<<(pin1*2)) | (3<<(pin2*2)));
    gpio->AFR[flagAFR1] &= ~((0xf << ((pin1 % 8) * 4)));
    gpio->AFR[flagAFR2] &= ~((0xf << ((pin2 % 8) * 4)));
    gpio->PUPDR &= ~((3 << (pin1 * 2)) | (3 << (pin2 * 2)));

    gpio->MODER |= (2 << (pin1 * 2)) | (2 << (pin2 * 2)); // alternate function
    gpio->AFR[flagAFR1] |= (alternateFunPin1 << ((pin1 % 8) * 4)); // alternate function pin1
    gpio->AFR[flagAFR2] |= (alternateFunPin2 << ((pin2 % 8) * 4));// alternate function pin2

    // timer configuration
//    timer->ARR = 0xFFFF - 1; // max counter value
    timer->ARR = 1000; // max counter value
    timer->SMCR |= (3 << 0); // encoder mode
    timer->CCMR1 |= (1 << 0) | (1 << 8); // compare channel selection
    timer->CCMR1 |= (filterOrder << 4) | (filterOrder << 12); // debouncing filter
    timer->CCER |= (1 << 0) | (1 << 4); // enabling compare channels
    timer->CR1 |= (TIM_CR1_CEN); // starting the timer

};

float Encoder::getValue() {
    lastValue = value;
    volatile uint32_t count = timer->CNT;
    // TODO: convert count to value
    return value;
}
