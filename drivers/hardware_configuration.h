
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
#define Button1Pin 9
#define Button2Port GPIOA
#define Button2Pin 9
#define Button3Port GPIOA
#define Button3Pin 9
#define Button4Port GPIOA
#define Button4Pin 9

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
#define DoubleEncoder1PortEnc GPIOA
#define DoubleEncoder1PinAEnc 0
#define DoubleEncoder1PinBEnc 1
#define DoubleEncoder1PortBut GPIOA
#define DoubleEncoder1PinBut 9
#define DoubleEncoder1Timer TIM2

// DoubleEncoder2
#define DoubleEncoder2PortEnc GPIOE
#define DoubleEncoder2PinAEnc 9
#define DoubleEncoder2PinBEnc 11
#define DoubleEncoder2PortBut GPIOA
#define DoubleEncoder2PinBut 9
#define DoubleEncoder2Timer TIM1

// DoubleEncoder3
#define DoubleEncoder3PortEnc GPIOB
#define DoubleEncoder3PinAEnc 5
#define DoubleEncoder3PinBEnc 4
#define DoubleEncoder3PortBut GPIOA
#define DoubleEncoder3PinBut 9
#define DoubleEncoder3Timer TIM3

// DoubleEncoder4
#define DoubleEncoder4PortEnc GPIOD
#define DoubleEncoder4PinAEnc 12
#define DoubleEncoder4PinBEnc 13
#define DoubleEncoder4PortBut GPIOA
#define DoubleEncoder4PinBut 9
#define DoubleEncoder4Timer TIM4

#endif //STM32_MONOSYNTH_HARDWARE_CONFIGURATION_H
