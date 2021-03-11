
#ifndef STM32_MONOSYNTH_SSD1306_H
#define STM32_MONOSYNTH_SSD1306_H

#include "miosix.h"
#include "../u8g2/u8g2.h"

#define DISPLAY_SCL_PIN 12
#define DISPLAY_SDA_PIN 13
#define DISPLAY_GPIO GPIOD
#define DISPLAY_GPIO_BASE GPIOD_BASE

class Ssd1306 {
public:
    Ssd1306() = delete;
    static void init();
private:
};

#endif //STM32_MONOSYNTH_SSD1306_H
