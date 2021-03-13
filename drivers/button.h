
#ifndef STM32_MONOSYNTH_BUTTON_H
#define STM32_MONOSYNTH_BUTTON_H

#include "miosix.h"

#include <functional>

enum class ButtonTriggeringType {
    RISING,
    FALLING
};

class Button {
public:
    Button(GPIO_TypeDef *gpio, uint8_t pin);

    Button(Button &) = delete;

    bool isPressed();

    void setTriggeringType(ButtonTriggeringType triggering);

    inline void setCallback(std::function<void(void)> newCallback) { callback = newCallback; };

    inline ButtonTriggeringType getButtonCallbackTriggering() { return triggeringType; };

    void update();

    Button &operator=(Button &) = delete;

private:
    GPIO_TypeDef *gpio;
    uint8_t gpioPin;
    bool previousState;
    std::function<void(void)> callback;
    ButtonTriggeringType triggeringType;
};


#endif //STM32_MONOSYNTH_BUTTON_H
