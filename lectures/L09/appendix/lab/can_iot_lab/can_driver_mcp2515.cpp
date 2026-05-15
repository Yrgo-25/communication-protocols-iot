/**
 * @brief CAN driver implementation details for the MCP2515 controller.
 */
#include <stdint.h>

#if 0
#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

#include "can_driver_mcp2515.h"
#include "can_frame.h"

namespace can::driver
{
/** @note Example implementation using a pointer to abstract the CAN controller. */
// -----------------------------------------------------------------------------
bool Mcp2515::init() noexcept
{
    // Check if memory allocation succeeded, terminate on failure.
    if (nullptr == myController)
    {
        Serial.println("Failed to initialize MCP2515 CAN driver: out of memory!");
        return false;
    }

    // Initialize CAN bus, set baud rate to 500 kbps.
    while (CAN_OK != myController->begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ))
    {
        constexpr uint8_t wait_ms{100U};
        Serial.print("CAN initialization failed, retrying in ");
        Serial.print(wait_ms);
        Serial.println(" ms...");
        delay(wait_ms);
    }
    myController->setMode(MCP_NORMAL);
    Serial.println("CAN sender initialization OK!");
    return true;
}
} // namespace can::driver

#endif
