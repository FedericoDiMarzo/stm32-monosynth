
#include "double_encoder.h"


DoubleEncoder::DoubleEncoder(int *timer, int *encoderGpio, int encoderPin1, int encoderPin2, int *buttonGpio,
                             int buttonPin) :
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

    newValue = newValue;
}