
#include "encoder.h"
#include "miosix.h"

using namespace miosix;

#define ALTERNATE_FUNCTION 2
#define PULL_UP 1
#define PULL_DOWN 2
#define RISING_EDGE 0
#define FALLING_EDGE 1
#define ENCODER_MODE_1 1
#define ENCODER_MODE_2 2
#define ENCODER_MODE_3 3


Encoder::Encoder(GPIO_TypeDef *gpio, unsigned int pin1, unsigned int pin2, TIM_TypeDef *timer) :
        _timer(timer),
        value(0.0f),
        lastValue(0.0f) {

    // Enabling clock
    // TODO: generalize to multiple timers and gpio
    uint32_t flagGpio = RCC_AHB1ENR_GPIOBEN;
    uint32_t flagTimer = RCC_APB2ENR_TIM1EN;
    RCC->AHB1ENR |= flagGpio;
    RCC->APB2ENR |= flagTimer;

    // GPIO configuration
    int lowHighAFR1 = (pin1 < 8) ? 0 : 1; // selecting AFR low or high register
    int lowHighAFR2 = (pin2 < 8) ? 0 : 1; // selecting AFR low or high register
    int timerAlternateFunctionIndex1 = 2; // TODO: switch case for each pin
    int timerAlternateFunctionIndex2 = 4;
    gpio->MODER |= (ALTERNATE_FUNCTION << (2 * pin1)) | // alternate function mode for pin1
                   (ALTERNATE_FUNCTION << (2 * pin2)); // alternate function mode for pin2
    gpio->AFR[lowHighAFR1] |= (timerAlternateFunctionIndex1 << ((pin1 % 8) * 4)); // choosing the AF for pin1
    gpio->AFR[lowHighAFR2] |= (timerAlternateFunctionIndex2 << ((pin2 % 8) * 4)); // choosing the AF for pin2
    gpio->PUPDR |= (PULL_UP << (pin1 * 2)) // pull-up mode for pin1
                   | (PULL_UP << (pin2 * 2)); // pull-up mode for pin2

    // TIMER configuration
    timer->ARR = 12048; // auto reload value
    timer->CCMR1 |= (1 << 0); // mapping channel 1 to comparator 1
    timer->CCMR1 |= TIM_CCMR1_CC2S_0; // mapping channel 2 to comparator 2
    timer->CCER |= (FALLING_EDGE << 1) | (FALLING_EDGE << 5); // falling edge mode for channels 1 and 2
//    timer->CCER |= (FALLING_EDGE << 3) | (FALLING_EDGE << 7); // falling edge mode for channels 1 and 2

    timer->SMCR |= ENCODER_MODE_3; // used to setup the encoder mode
    timer->CNT = 100; // resets the count
    timer->CR1 |= TIM_CR1_CEN; // counter enabled

};

float Encoder::getValue() {
    lastValue = value;
    int count = _timer->CNT;
    // TODO: convert count to value
    return value;
}

void Encoder::testStaticRotary() {
    typedef Gpio<GPIOD_BASE, 12> rotPin1;
    typedef Gpio<GPIOD_BASE, 13> rotPin2;

    RCC->APB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // GPIO
    rotPin1::mode(Mode::ALTERNATE);
    rotPin2::mode(Mode::ALTERNATE);
    rotPin1::alternateFunction(2);
    rotPin2::alternateFunction(2);

    // TIM4
    int filterOrder = 8;
    TIM4->ARR = 0xFFFF-1;
//    TIM4->EGR |= TIM_EGR_UG;
    TIM4->SMCR |= (3 << 0); // encoder mode
    TIM4->CCMR1 |= (1 << 0) | (1 << 8); // compare channel selection
    TIM4->CCMR1 |= (filterOrder << 4) | (filterOrder << 12); // debouncing filter
    TIM4->CCER |= (1 << 0) | (1 << 4); // enabling compare channels
    TIM4->CR1 |= (TIM_CR1_CEN); // starting the timer

    volatile int count = 0;
    while (true) {
        count = TIM4->CNT;
    }

}
