/**
 * @brief CAN IoT lab definitions.
 */
#pragma once

#ifdef STUB
// Include stub driver headers when testing.
#include "can_driver_stub.h"
#include "gpio_stub.h"
#include "sim_button.h"
#else
// Include real driver headers when running on real hardware.
#include "can_driver_mcp2515.h"
#include "gpio_arduino.h"
#endif /** STUB */

#ifdef STUB
/** Select stub drivers for testing without GPIO/CAN hardware. */
using CanDriver = can::driver::Stub;
using Gpio      = gpio::Stub;
#else
/** Select real drivers for testing with GPIOs and a CAN controller. */
using CanDriver = can::driver::Mcp2515;
using Gpio      = gpio::Arduino;
#endif /** STUB */
