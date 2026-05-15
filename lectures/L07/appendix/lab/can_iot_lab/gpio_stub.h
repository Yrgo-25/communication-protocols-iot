/**
 * @brief GPIO driver stub.
 */
#pragma once

#include <stdint.h>

#include "gpio_interface.h"

namespace gpio
{
/**
 * @brief GPIO driver stub.
 *
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] pin GPIO pin.
     * @param[in] mode GPIO mode.
     */
    explicit Stub(uint8_t pin, Mode mode) noexcept
        : myPin{pin}
        , myMode{mode}
        , myEvent{false}
    {}

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Check if the GPIO is initialized.
     *
     * @return True if initialized, false othewise.
     */
    bool isInitialized() const noexcept override { return true; }

    /**
     * @brief Get GPIO pin.
     *
     * @return GPIO pin.
     */
    uint8_t pin() const noexcept override { return myPin; }

    /**
     * @brief Get GPIO mode.
     *
     * @return Configured GPIO mode.
     */
    Mode mode() const noexcept override { return myMode; }

    /**
     * @brief Set GPIO state.
     *
     * @param[in] state GPIO state (true = high, false = low).
     */
    void write(const bool state) noexcept override 
    { 
        myEvent = myState != state;
        myState = state; 
    }

    /**
     * @brief Read GPIO state.
     *
     * @return True if high, false if low.
     */
    bool read() const noexcept override { return myState; }

    /**
     * @brief Toggle GPIO state.
     */
    void toggle() noexcept override 
    { 
        myState = !myState;
        myEvent = true;
    }

    /**
     * @brief Check if a GPIO event has occurred since last read.
     * 
     * @return True on event, false otherwise.
     */
    bool event() noexcept override 
    { 
        const bool event{myEvent};
        myEvent = false;
        return event;
    }

    Stub()                          = delete; // No default constructor.
    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment..
    Stub& operator=(Stub&&)      = delete; // No move assignment. 
    
private:
    /** GPIO pin. */
    const uint8_t myPin;

    /** GPIO mode. */
    const Mode myMode;

    /** GPIO state. */
    bool myState;

    /** True on event, false otherwise. */
    bool myEvent;
};
} // namespace
