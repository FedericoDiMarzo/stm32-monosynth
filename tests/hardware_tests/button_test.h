
#ifndef STM32_MONOSYNTH_BUTTON_TEST_H
#define STM32_MONOSYNTH_BUTTON_TEST_H

#include "../../drivers/hardware_configuration.h"
#include "../../drivers/button.h"

/**
 * Enable this flag to test the switchedOn/Off methods.
 */
//#define BUTTON_TEST_TRIGGERING

/**
 * Test for the button driver.
 */
void buttonTest() {
    Button button1(Button1Port, Button1Pin);
    Button button2(Button2Port, Button2Pin);
    Button button3(Button3Port, Button3Pin);
    Button button4(Button4Port, Button4Pin);
    volatile bool button1State;
    volatile bool button2State;
    volatile bool button3State;
    volatile bool button4State;
#ifndef BUTTON_TEST_TRIGGERING // testing the internal state
    while (true) {
        button1.update();

        button1State = button1.isPressed();
        button2State = button2.isPressed();
        button3State = button3.isPressed();
        button4State = button4.isPressed();

        miosix::Thread::sleep(20);
    }
#else // testing triggering
    while (true) {
        button1.update();

        if (button1.switchedOn() | button2.switchedOn() |
        button3.switchedOn() | button4.switchedOn()) {
            miosix::Thread::sleep(20);
        }

        if (button1.switchedOff() | button2.switchedOff() |
            button3.switchedOff() | button4.switchedOff()) {
             miosix::Thread::sleep(20);
        }

        miosix::Thread::sleep(BUTTON_UPDATE_PERIOD);
    }
#endif
}


#endif //STM32_MONOSYNTH_BUTTON_TEST_H
