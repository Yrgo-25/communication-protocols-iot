/**
 * @brief Implementation details of GPIO wrapper for Arduino boards.
 */
#include <stdint.h>

#include <Arduino.h>

#include "gpio_arduino.h"

namespace gpio
{
namespace
{
// -----------------------------------------------------------------------------
void configurePin(const uint8_t pin, const Mode mode) noexcept
{
    switch (mode)
    {
        case Mode::InputPullup:
            pinMode(pin, INPUT_PULLUP);
            break;
        case Mode::Output:
            pinMode(pin, OUTPUT);
            break;
        default:
            pinMode(pin, INPUT);
    }
}
} // namespace

// -----------------------------------------------------------------------------
Arduino::Arduino(const uint8_t pin, const Mode mode) noexcept
    : myPin{pin}
    , myMode{mode}
    , myState{false}
{
    // Configure pin as specified.
    configurePin(pin, mode);
}

// -----------------------------------------------------------------------------
Arduino::~Arduino() noexcept
{
    // Reset pin before destruction.
    digitalWrite(myPin, false);
    pinMode(myPin, INPUT);
}

// -----------------------------------------------------------------------------
bool Arduino::isInitialized() const noexcept 
{
    // Not implemented for this wrapper, always return true.
    return true;
}

// -----------------------------------------------------------------------------
uint8_t Arduino::pin() const noexcept { return myPin; }

// -----------------------------------------------------------------------------
Mode Arduino::mode() const noexcept { return myMode; }

// -----------------------------------------------------------------------------
void Arduino::write(const bool state) noexcept 
{
    // Check mode, ignore if not configured as output.
    if (Mode::Output != myMode) { return; }

    // Set new state.
    digitalWrite(myPin, state);
}

// -----------------------------------------------------------------------------
bool Arduino::read() const noexcept 
{ 
    // Read GPIO pin, return true if high, false otherwise.
    return static_cast<bool>(digitalRead(myPin)); 
}

// -----------------------------------------------------------------------------
void Arduino::toggle() noexcept { write(!read()); }

// -----------------------------------------------------------------------------
bool Arduino::event() noexcept
{
    const bool state{read()};
    const bool event{state != myState};
    myState = state;
    return event;
}
} // namespace
