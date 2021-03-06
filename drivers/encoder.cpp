
#include "miosix.h"
#include "encoder.h"
#include "core_util.h"

using namespace miosix;

Encoder::Encoder(TIM_TypeDef *timer, GPIO_TypeDef *gpio, uint8_t pin1, uint8_t pin2) :
        timer(timer),
        value(0.0f),
        sensitivity(1.0f) {

    uint8_t alternateFunPin1 = CoreUtil::getGpioTimerAf(gpio, pin1);
    uint8_t alternateFunPin2 = CoreUtil::getGpioTimerAf(gpio, pin2);

    int filterOrder = 8;
    int flagAFR1 = (pin1 < 8) ? 0 : 1;
    int flagAFR2 = (pin2 < 8) ? 0 : 1;

    {
        // disabling interrupts during configuration
        miosix::FastInterruptDisableLock dLock;

        CoreUtil::rccEnableGpio(gpio);
        CoreUtil::rccEnableTimer(timer);

        // gpio configuration
        gpio->MODER &= ~((3 << (pin1 * 2)) | (3 << (pin2 * 2)));
        gpio->AFR[flagAFR1] &= ~((0xf << ((pin1 % 8) * 4)));
        gpio->AFR[flagAFR2] &= ~((0xf << ((pin2 % 8) * 4)));
        gpio->PUPDR &= ~((3 << (pin1 * 2)) | (3 << (pin2 * 2)));
        gpio->OTYPER &= ~((1 << pin1) | (1 << pin2));

        gpio->MODER |= (2 << (pin1 * 2)) | (2 << (pin2 * 2)); // alternate function
        gpio->AFR[flagAFR1] |= (alternateFunPin1 << ((pin1 % 8) * 4)); // alternate function pin1
        gpio->AFR[flagAFR2] |= (alternateFunPin2 << ((pin2 % 8) * 4));// alternate function pin2
        gpio->PUPDR |= ((2 << (pin1 * 2)) | (2 << (pin2 * 2))); // pull-down

        // timer configuration
        *timer = {};
        timer->ARR = arrValue; // max counter value
        timer->SMCR |= (3 << 0); // encoder mode
        timer->CCMR1 |= (1 << 0) | (1 << 8); // compare channel selection
        timer->CCMR1 |= (filterOrder << 4) | (filterOrder << 12); // debouncing filter
        timer->CCER |= (1 << 0) | (1 << 4); // enabling compare channels
        timer->CNT = arrValue / 2; // count starts at middle point for max excursion
        timer->CR1 |= (TIM_CR1_CEN); // starting the timer
    }

};


void Encoder::update() {
    value += getIncrement();

    // clipping between 0 and 1
    value = (value < 0.0f) ? 0.0f : value;
    value = (value > 1.0f) ? 1.0f : value;
}

void Encoder::setValue(float newValue) {
    // clipping
    newValue = (newValue < 0) ? 0 : newValue;
    newValue = (newValue > 1) ? 1 : newValue;

    value = newValue;
}

float Encoder::getIncrement() const {
    // getting the count from the CNT register
    volatile uint32_t count = timer->CNT;

    // conversion to signed int
    int32_t countSigned = static_cast<int32_t>(count);

    // the last count is always reset to the middle point
    // at the end of the function
    int32_t lastCount = static_cast<int32_t>(arrValue / 2);

    {
        // disabling the interrupts ad resetting the
        // count to the middle point
        miosix::FastInterruptDisableLock dLock;
        timer->CNT = arrValue / 2;
    }

    // the delta between the middle point of the timer count
    // and the count at the function call is calculated and
    // scaled by the sensitivity
    return static_cast<float>(countSigned - lastCount) / static_cast<float>(arrValue)
           * sensitivity * ENCODER_SENSITIVITY_OFFSET;

}

