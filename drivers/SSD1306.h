
#ifndef STM32_MONOSYNTH_SSD1306_H
#define STM32_MONOSYNTH_SSD1306_H

#include "miosix.h"
#include "../u8g2/u8g2.h"

#define DISPLAY_SCL_PIN 10
#define DISPLAY_SDA_PIN 11
#define DISPLAY_GPIO GPIOB
#define DISPLAY_GPIO_BASE GPIOB_BASE
#define DISPLAY_I2C I2C2

/**
 * Drivers for ssd1306 display, based on the library U8g2lib
 * https://github.com/olikraus/u8g2
 */
class Ssd1306 {
public:
    Ssd1306() = delete;
    static void init();
private:
};

#endif //STM32_MONOSYNTH_SSD1306_H
