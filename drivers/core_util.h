
#ifndef STM32_MONOSYNTH_CORE_UTIL_H
#define STM32_MONOSYNTH_CORE_UTIL_H

#include "miosix.h"

/**
 * Used to as an invalid result for an
 * alternate function getter function.
 */
#define INVALID_AF 0xf

/**
 * This module defines hardware specific functions for the
 * board stm32f407vg discovery.
 *
 * To provide compatibility to other boards, provide a custom
 * implementation of the cpp file.
 */
namespace CoreUtil {
    /**
     * Enables the clock for a GPIO.
     *
     * @param gpio GPIO_TypeDef pointer
     */
    void rccEnableGpio(const GPIO_TypeDef *gpio);

    /**
     * Enables the clock for a timer.
     *
     * @param timer TIM_TypeDef pointer
     */
    void rccEnableTimer(const TIM_TypeDef *timer);

    /**
     * Enable the clock for a i2c unit.
     *
     * @param i2c I2C_TypeDef pointer
     */
    void rccEnableI2c(const I2C_TypeDef *i2c);

    /**
     * Enable the clock for a USART unit.
     *
     * @param usart USART_TypeDef pointer
     */
    void rccEnableUsart(const USART_TypeDef *usart);

    /**
     * Gets the timer alternate function for a certain pin, just for
     * pins having a TIMxCH1/CH2 routing.
     *
     * @param gpio target GPIO
     * @param pin target PIN
     * @return alternate function index
     */
    uint8_t getGpioTimerAf(const GPIO_TypeDef *gpio, uint8_t pin);

    /**
     * Gets the I2C alternate function for a certain pin, just for
     * pins having a SDA/SCL routing.
     *
     * @param gpio target GPIO
     * @param pin target PIN
     * @return alternate function index
     */
    uint8_t getGpioI2cAf(const GPIO_TypeDef *gpio, uint8_t pin);

    /**
     * Gets the USART alternate function for a certain pin, just for
     * pins having a USART routing.
     *
     * @param gpio target GPIO
     * @param pin target PIN
     * @return alternate function index
     */
    uint8_t getUsartAf(const GPIO_TypeDef *gpio, uint8 pin);

    /**
     * This function implements an infinite loop,
     * It can be used to debug errors.
     */
    void errorHandler();
}

#endif //STM32_MONOSYNTH_CORE_UTIL_H
