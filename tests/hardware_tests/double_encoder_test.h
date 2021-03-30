
#ifndef STM32_MONOSYNTH_DOUBLE_ENCODER_TEST_H
#define STM32_MONOSYNTH_DOUBLE_ENCODER_TEST_H

#include "../../drivers/hardware_configuration.h"
#include "../../drivers/double_encoder.h"


/**
 * Test for the double encoder driver.
 */
void doubleEncoderTest() {
    DoubleEncoder doubleEncoder1(DoubleEncoder1Timer, DoubleEncoder1PortEnc,
                                 DoubleEncoder1PinAEnc, DoubleEncoder1PinBEnc,
                                 DoubleEncoder1PortBut, DoubleEncoder1PinBut);
    DoubleEncoder doubleEncoder2(DoubleEncoder2Timer, DoubleEncoder2PortEnc,
                                 DoubleEncoder2PinAEnc, DoubleEncoder2PinBEnc,
                                 DoubleEncoder2PortBut, DoubleEncoder2PinBut);
    DoubleEncoder doubleEncoder3(DoubleEncoder3Timer, DoubleEncoder3PortEnc,
                                 DoubleEncoder3PinAEnc, DoubleEncoder3PinBEnc,
                                 DoubleEncoder3PortBut, DoubleEncoder3PinBut);
    DoubleEncoder doubleEncoder4(DoubleEncoder4Timer, DoubleEncoder4PortEnc,
                                 DoubleEncoder4PinAEnc, DoubleEncoder4PinBEnc,
                                 DoubleEncoder4PortBut, DoubleEncoder4PinBut);


    float sensitivity = 0.5;
    doubleEncoder1.setSensitivity(sensitivity);

    volatile float value1;
    volatile float value2;
    volatile float value3;
    volatile float value4;
    volatile float pressedValue1;
    volatile float pressedValue2;
    volatile float pressedValue3;
    volatile float pressedValue4;

    doubleEncoder1.setValue(0.5);
    doubleEncoder2.setValue(0.5);
    doubleEncoder3.setValue(0.5);
    doubleEncoder4.setValue(0.5);

    doubleEncoder1.setPressedValue(0.5);
    doubleEncoder2.setPressedValue(0.5);
    doubleEncoder3.setPressedValue(0.5);
    doubleEncoder4.setPressedValue(0.5);

    while (true) {
        doubleEncoder1.update();
        doubleEncoder2.update();
        doubleEncoder3.update();
        doubleEncoder4.update();

        value1 = doubleEncoder1.getValue();
        value2 = doubleEncoder2.getValue();
        value3 = doubleEncoder3.getValue();
        value4 = doubleEncoder4.getValue();

        pressedValue1 = doubleEncoder1.getPressedValue();
        pressedValue2 = doubleEncoder2.getPressedValue();
        pressedValue3 = doubleEncoder3.getPressedValue();
        pressedValue4 = doubleEncoder4.getPressedValue();

        miosix::Thread::sleep(20);
    }
}

#endif //STM32_MONOSYNTH_DOUBLE_ENCODER_TEST_H
