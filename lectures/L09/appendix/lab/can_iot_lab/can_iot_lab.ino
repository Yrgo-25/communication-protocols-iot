/**
 * @brief Arduino CAN lab application using the MCP2515 CAN controller.
 */
#include <stdint.h>

/**
 * @brief Enable stub drivers for testing without external hardware.
 * 
 * @note Comment out when using GPIOs and a CAN controller.
 */
#define STUB

#include "can_iot_lab.h"
#include "sys_logic.h"

namespace
{
/** LED 1. */
Gpio led1{5U, gpio::Mode::Output};

/** LED 2. */
Gpio led2{6U, gpio::Mode::Output};

/** Button 1. */
Gpio button1{7U, gpio::Mode::InputPullup};

/** Button 2. */
Gpio button2{8U, gpio::Mode::InputPullup};

/** CAN driver. */
CanDriver canDriver{9U};

/** System configuration. */
sys::Config config{115200UL, 0x0AUL, 0x0BUL};

/** System logic. */
sys::Logic logic{canDriver, led1, led2, button1, button2, config};

/**
 * @brief Periodically simulate button events for testing.
 * 
 * @note Only supported when using stubs.
 */
void simulateButtonEvents() noexcept
{
#ifdef STUB
    static sim::Button buttonSim1{5000UL, button1};
    static sim::Button buttonSim2{8000UL, button2};
    buttonSim1.simulateEvent();
    buttonSim2.simulateEvent();
#endif /** STUB */
}
} // namespace

/**
 * @brief Initialize system.
 */
void setup() 
{
    logic.init();
}

/**
 * @brief Run system.
 */
void loop()
{
    simulateButtonEvents();
    logic.run();
}
