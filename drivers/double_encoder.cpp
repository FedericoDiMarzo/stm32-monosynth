
#include "double_encoder.h"


DoubleEncoder::DoubleEncoder(TIM_TypeDef *timer,
                             GPIO_TypeDef *encoderGpio,
                             uint8_t encoderPin1,
                             uint8_t encoderPin2,
                             GPIO_TypeDef *buttonGpio,
                             uint8_t buttonPin) :
        Encoder(timer, encoderGpio, encoderPin1, encoderPin2),
        button(buttonGpio, buttonPin),
        pressedValue(0) {};

void DoubleEncoder::update() {

    if (button.isPressed()) {
        // incrementing the secondary value
        pressedValue += getIncrement();
        // clipping between 0 and 1
        pressedValue = (pressedValue < 0.0f) ? 0.0f : pressedValue;
        pressedValue = (pressedValue > 1.0f) ? 1.0f : pressedValue;
    } else {
        // incrementing the main value
        value += getIncrement();
        // clipping between 0 and 1
        value = (value < 0.0f) ? 0.0f : value;
        value = (value > 1.0f) ? 1.0f : value;
    }
}

void DoubleEncoder::setPressedValue(float newValue) {
    // clipping
    newValue = (newValue < 0) ? 0 : newValue;
    newValue = (newValue > 1) ? 1 : newValue;

    pressedValue = newValue;
}