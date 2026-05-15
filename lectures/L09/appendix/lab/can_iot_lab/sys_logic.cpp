/**
 * @brief System logic implementation details.
 */
#include <stdint.h>

#include <Arduino.h>

#include "can_driver_interface.h"
#include "can_frame.h"
#include "gpio_interface.h"
#include "sys_logic.h"

namespace sys
{
namespace
{
// -----------------------------------------------------------------------------
void logButtonState(const uint8_t pin, const bool state) noexcept
{
    Serial.print("\nButton at pin ");
    Serial.print(pin);
    const char* stateMsg{state ? " pressed!" : " released!"};
    Serial.println(stateMsg);
}

// -----------------------------------------------------------------------------
void logFrameInfo(const can::Frame& frame, const char* start = nullptr) noexcept
{
    if (nullptr != start) { Serial.print(start); }
    Serial.print("ID = ");
    Serial.print(frame.id);
    Serial.print(", dlc = ");
    Serial.print(frame.dlc);
    Serial.print(", data = {");
    
    for (uint8_t i{}; i < frame.dlc; ++i)
    {
        Serial.print(frame.data[i]);
        if ((frame.dlc - 1U) > i) { Serial.print(", ");}
    }
    Serial.println("}!");
}

// -----------------------------------------------------------------------------
void logLedState(const gpio::Interface& led) noexcept
{
    Serial.print("LED at pin ");
    Serial.print(led.pin());
    const char* stateMsg{led.read() ? " enabled!" : " disabled!"};
    Serial.println(stateMsg);
}
} // namespace

// -----------------------------------------------------------------------------
Logic::Logic(can::driver::Interface& canDriver, gpio::Interface& led1, gpio::Interface& led2,
             gpio::Interface& button1, gpio::Interface& button2, const Config& config) noexcept
    : myCanDriver{canDriver}
    , myLed1{led1}
    , myLed2{led2}
    , myButton1{button1}
    , myButton2{button2}
    , myConfig{config}
{}

// -----------------------------------------------------------------------------
bool Logic::init() noexcept
{
    // Initialize serial transmission.
    Serial.begin(myConfig.baudRate_bps);
    while (!Serial);

    // Initialize CAN driver, return result.
    return myCanDriver.init();
}

// -----------------------------------------------------------------------------
void Logic::run() noexcept
{
    constexpr uint8_t wait_ms{100U};

    // Check for button events, handle if present.
    const bool event1{handleButtonEvent(myButton1, myConfig.button1StateId)};
    handleButtonEvent(myButton2, myConfig.button2StateId);

    // Update LED1 state on button 1 event.
    if (event1)
    {
        const bool state{myButton1.read()};
        myLed1.write(state);
    }
    // Wait for 100 ms, then read incoming data.
    delay(wait_ms);
    readIncomingData();
}

// -----------------------------------------------------------------------------
bool Logic::handleButtonEvent(gpio::Interface& button, const uint32_t canId) noexcept
{
    // Look for button event.
    const bool event{button.event()};

    // Transmit CAN frame with new state if an event occurred.
    if (event)
    {
        const bool state{button.read()};
        logButtonState(button.pin(), state);

        // Create CAN frame.
        can::Frame frame{canId};
        frame.dlc      = sizeof(state);
        frame.data[0U] = static_cast<uint8_t>(state);

        // Transmit CAN frame.
        logFrameInfo(frame, "Sending CAN frame with ");
        myCanDriver.send(frame);
        
    }
    // Return true if an event occurred, false otherwise.
    return event;
}

// -----------------------------------------------------------------------------
void Logic::readIncomingData() noexcept
{
    // Check if data is available, handle each received byte on by one.
    while (myCanDriver.isDataAvailable()) 
    {
        // Read button state, handle if present.
        can::Frame frame{};
        const bool stateReceived{myCanDriver.receive(frame)};
        if (!stateReceived) { continue; }
        logFrameInfo(frame, "Received CAN frame with ");
        
        // Parse state from the payload.
        constexpr uint8_t stateIdx{0U};
        const uint8_t state{frame.data[stateIdx]};

        // Check CAN ID, update LED states on receive from the corresponding buttons.
        if (myConfig.button1StateId == frame.id)
        {
            myLed1.write(state);
            logLedState(myLed1);
        }
        else if (myConfig.button2StateId == frame.id)
        {
            myLed2.write(state);
            logLedState(myLed2);
        }
        else
        {
            Serial.print("Received unknown CAN frame with ID ");
            Serial.print(frame.id);
            Serial.println();
        }
    }
}
} // namespace sys
