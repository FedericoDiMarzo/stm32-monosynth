
#ifndef STM32_MONOSYNTH_CORE_UTIL_H
#define STM32_MONOSYNTH_CORE_UTIL_H

#include "miosix.h"
#define INVALID_AF 0xf

namespace CoreUtil {
    void rccEnableGpio(GPIO_TypeDef *gpio);

    void rccEnableTimer(TIM_TypeDef *timer);

    uint32_t gpioGetTimerAF(GPIO_TypeDef *gpio, uint32_t pin);
}

#endif //STM32_MONOSYNTH_CORE_UTIL_H
