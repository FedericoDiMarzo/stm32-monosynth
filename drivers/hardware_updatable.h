
#ifndef STM32_MONOSYNTH_HARDWARE_UPDATABLE_H
#define STM32_MONOSYNTH_HARDWARE_UPDATABLE_H

/**
 * Interface implemented in the hardware drivers when
 * a constant update from a thread is needed.
 */
class HardwareUpdatable {
public:

    /**
     * This update function can be called from a specific hardware
     * thread in order to update the internal state of a driver.
     */
    virtual void update() = 0;
};

#endif //STM32_MONOSYNTH_HARDWARE_UPDATABLE_H
