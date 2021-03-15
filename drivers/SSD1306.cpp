

#include "miosix.h"
#include "ssd1306.h"
#include "../u8g2/u8g2.h"
#include "../drivers/core_util.h"
#include "../miosix/util/software_i2c.h"

/**
 * Pin used for i2c serial clock.
 */
typedef miosix::Gpio<DISPLAY_GPIO_BASE, DISPLAY_SCL_PIN> scl;

/**
 * Pin used for i2c data line.
 */
typedef miosix::Gpio<DISPLAY_GPIO_BASE, DISPLAY_SDA_PIN> sda;

/**
 * I2c interface.
 */
typedef miosix::SoftwareI2C<sda, scl> i2c;

/**
 * U8g2 display library object.
 */
static u8g2_t u8g2;


/**
 * This function is used to implement HAL interface needed by the u8g2 library.
 * It's used to handle the GPIOs and timing.
 *
 * @param u8x8
 * @param msg
 * @param arg_int
 * @param arg_ptr
 * @return
 */
uint8_t u8x8_gpio_and_delay_implementation(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    float dlyUs;
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:    // called once during init phase of u8g2/u8x8
            break;                            // can be used to setup pins
        case U8X8_MSG_DELAY_NANO:            // delay arg_int * 1 nano second
            break;
        case U8X8_MSG_DELAY_100NANO:        // delay arg_int * 100 nano seconds
            break;
        case U8X8_MSG_DELAY_10MICRO:        // delay arg_int * 10 micro seconds
            miosix::delayUs(10 * arg_int);
            break;
        case U8X8_MSG_DELAY_MILLI:            // delay arg_int * 1 milli second
            miosix::delayMs(arg_int);
            break;
        case U8X8_MSG_DELAY_I2C:
            // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
            dlyUs = 5 / static_cast<float>(arg_int);
            miosix::delayUs(static_cast<int>(dlyUs));
            break;
        case U8X8_MSG_GPIO_D0:                // D0 or SPI clock pin: Output level in arg_int
            break;                            //case U8X8_MSG_GPIO_SPI_CLOCK:
        case U8X8_MSG_GPIO_D1:                // D1 or SPI data pin: Output level in arg_int
            break;                            //case U8X8_MSG_GPIO_SPI_DATA:
        case U8X8_MSG_GPIO_D2:                // D2 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D3:                // D3 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D4:                // D4 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D5:                // D5 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D6:                // D6 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D7:                // D7 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_E:                // E/WR pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS:                // CS (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_DC:                // DC (data/cmd, A0, register select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_RESET:            // Reset pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS1:                // CS1 (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS2:                // CS2 (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_I2C_CLOCK:
            break;
        case U8X8_MSG_GPIO_I2C_DATA:
            break;
        case U8X8_MSG_GPIO_MENU_SELECT:
            u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_NEXT:
            u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_PREV:
            u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
            break;
        default:
            u8x8_SetGPIOResult(u8x8, 1);            // default return value
            break;
    }
    return 1;
}

/**
 * This function is used to implement HAL interface needed by the u8g2 library.
 * It's used to handle i2c communication.
 *
 * @param u8x8
 * @param msg
 * @param arg_int
 * @param arg_ptr
 * @return
 */
uint8_t u8x8_byte_hw_i2c_implementation(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    //  u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER
    uint8_t *data;

    switch (msg) {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *) arg_ptr;
            while (arg_int > 0) {
                i2c::init();
                i2c::send(*data);
                data++;
                arg_int--;
            }
            break;
        case U8X8_MSG_BYTE_INIT:
            /* add your custom code to init i2c subsystem */
            i2c::init();
            break;
        case U8X8_MSG_BYTE_SET_DC:
            /* ignored for i2c */
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            i2c::sendStart();
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            i2c::sendStop();
            break;
        default:
            return 0;
    }
    return 1;
}

void testScreen() {
    u8g2_FirstPage(&u8g2);
    do {
        u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
        u8g2_DrawStr(&u8g2, 0, 24, "Hi World!");
        u8g2_SendBuffer(&u8g2);
    } while (u8g2_NextPage(&u8g2));
}

/**
 * Function used to setup the display pins and i2c
 */
void setupGpioDisplay() {
    miosix::FastInterruptDisableLock dLock;

    CoreUtil::rccEnableGpio(DISPLAY_GPIO);
    CoreUtil::rccEnableI2c(DISPLAY_I2C);
    uint8_t sclAf = CoreUtil::getGpioI2cAf(DISPLAY_GPIO, DISPLAY_SCL_PIN);
    uint8_t sdaAf = CoreUtil::getGpioI2cAf(DISPLAY_GPIO, DISPLAY_SDA_PIN);

    i2c::init();
    sda::alternateFunction(sdaAf);
    scl::alternateFunction(sclAf);
}

void Ssd1306::init() {
    setupGpioDisplay();
    u8g2_Setup_ssd1306_i2c_128x32_winstar_1(&u8g2, U8G2_R0, u8x8_byte_hw_i2c_implementation,
                                            u8x8_gpio_and_delay_implementation);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    testScreen();
}