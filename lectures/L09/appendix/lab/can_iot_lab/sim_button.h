/**
 * @brief Button simulator implementation.
 */
#pragma once

#include <stdint.h>

#include <Arduino.h>

#include "gpio_interface.h"

namespace sim
{
/**
 * @brief Button simulator implementation for testing.
 */
class Button final
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] timeout_ms Timeout in milliseconds.
     * @param[in] button Button to simulate.
     */
    explicit Button(const uint32_t timeout_ms, gpio::Interface& button) noexcept
        : myTimeout_ms{timeout_ms}
        , myStart{millis()}
        , myButton{button}
    {}

    /**
     * @brief Destructor.
     */
    ~Button() noexcept = default;

    /**
     * @brief Toggle button state on timeout.
     */
    void simulateEvent() noexcept
    {
        // Compute elapsed time since last button toggle.
        const auto now      = millis();
        const auto delta_ms = now - myStart;

        // Toggle button and store new timestamp on timeout.
        if (myTimeout_ms <= delta_ms)
        {
            myButton.toggle();
            myStart = now;
        }
    }

    Button()                         = delete; // No default constructor.
    Button(const Button&)            = delete; // No copy constructor.
    Button(Button&&)                 = delete; // No move constructor.
    Button& operator=(const Button&) = delete; // No copy assignment.
    Button& operator=(Button&&)      = delete; // No move assignment.

private:
    /** Timeout in milliseconds. */
    const uint32_t myTimeout_ms;

    /** Start timestamp. */
    uint32_t myStart;

    /** Button to simulate. */
    gpio::Interface& myButton;
};
} // namespace sim
