
#ifndef STM32_MONOSYNTH_HARDWARE_CONFIGURATION_H
#define STM32_MONOSYNTH_HARDWARE_CONFIGURATION_H

#include "miosix.h"

/**
 * This is the header that defines the harware configuration
 * in relation to the code. Check the pin_layout.txt file for
 * additional information.
 */

// Buttons
#define Button1Port GPIOA
#define Button1Pin 3
#define Button2Port GPIOA
#define Button2Pin 5
#define Button3Port GPIOA
#define Button3Pin 7
#define Button4Port GPIOC
#define Button4Pin 5

// Encoders
#define EncoderSensitivity 0.5f

// Encoder1
#define Encoder1Port GPIOA
#define Encoder1PinA 0
#define Encoder1PinB 1
#define Encoder1Timer TIM2

// Encoder2
#define Encoder2Port GPIOE
#define Encoder2PinA 9
#define Encoder2PinB 11
#define Encoder2Timer TIM1

// Encoder3
#define Encoder3Port GPIOB
#define Encoder3PinA 5
#define Encoder3PinB 4
#define Encoder3Timer TIM3

// Encoder4
#define Encoder4Port GPIOD
#define Encoder4PinA 12
#define Encoder4PinB 13
#define Encoder4Timer TIM4

// DoubleEncoder1
#define DoubleEncoder1PortEnc Encoder1Port
#define DoubleEncoder1PinAEnc Encoder1PinA
#define DoubleEncoder1PinBEnc Encoder1PinB
#define DoubleEncoder1PortBut Button1Port
#define DoubleEncoder1PinBut Button1Pin
#define DoubleEncoder1Timer Encoder1Timer

// DoubleEncoder2
#define DoubleEncoder2PortEnc Encoder2Port
#define DoubleEncoder2PinAEnc Encoder2PinA
#define DoubleEncoder2PinBEnc Encoder2PinB
#define DoubleEncoder2PortBut Button2Port
#define DoubleEncoder2PinBut Button2Pin
#define DoubleEncoder2Timer Encoder2Timer

// DoubleEncoder3
#define DoubleEncoder3PortEnc Encoder3Port
#define DoubleEncoder3PinAEnc Encoder3PinA
#define DoubleEncoder3PinBEnc Encoder3PinB
#define DoubleEncoder3PortBut Button3Port
#define DoubleEncoder3PinBut Button3Pin
#define DoubleEncoder3Timer Encoder3Timer

// DoubleEncoder4
#define DoubleEncoder4PortEnc Encoder4Port
#define DoubleEncoder4PinAEnc Encoder4PinA
#define DoubleEncoder4PinBEnc Encoder4PinB
#define DoubleEncoder4PortBut Button4Port
#define DoubleEncoder4PinBut Button4Pin
#define DoubleEncoder4Timer Encoder4Timer

#endif //STM32_MONOSYNTH_HARDWARE_CONFIGURATION_H
