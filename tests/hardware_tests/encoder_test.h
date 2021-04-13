
#ifndef STM32_MONOSYNTH_ENCODER_TEST_H
#define STM32_MONOSYNTH_ENCODER_TEST_H

#include "../../drivers/hardware_configuration.h"
#include "../../drivers/encoder.h"


/**
 * Test for the encoder driver.
 */
void encoderTest() {
    Encoder encoder1(Encoder1Timer, Encoder1Port, Encoder1PinA, Encoder1PinB);
    Encoder encoder2(Encoder2Timer, Encoder2Port, Encoder2PinA, Encoder2PinB);
    Encoder encoder3(Encoder3Timer, Encoder3Port, Encoder3PinA, Encoder3PinB);
    Encoder encoder4(Encoder4Timer, Encoder4Port, Encoder4PinA, Encoder4PinB);

    encoder1.setSensitivity(EncoderSensitivity);
    encoder2.setSensitivity(EncoderSensitivity);
    encoder3.setSensitivity(EncoderSensitivity);
    encoder4.setSensitivity(EncoderSensitivity);

    volatile float value1;
    volatile float value2;
    volatile float value3;
    volatile float value4;

    encoder1.setValue(0.5);
    encoder2.setValue(0.5);
    encoder3.setValue(0.5);
    encoder4.setValue(0.5);

    while (true) {
        encoder1.update();
        encoder2.update();
        encoder3.update();
        encoder4.update();

        value1 = encoder1.getValue();
        value2 = encoder2.getValue();
        value3 = encoder3.getValue();
        value4 = encoder4.getValue();

        miosix::Thread::sleep(20);
    }
}

#endif //STM32_MONOSYNTH_ENCODER_TEST_H
