/**
 * @brief GPIO interface.
 */
#pragma once

#include <stdint.h>

namespace gpio
{
/**
 * @brief Enumeration of GPIO modes.
 */
enum class Mode : uint8_t
{
    Input,       ///< GPIO input.
    InputPullup, ///< GPIO input with internal pull-up resistor enabled.
    Output,      ///< GPIO output.
};

/**
 * @brief GPIO interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Check if the GPIO is initialized.
     *
     * @return True if initialized, false othewise.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Get GPIO pin.
     *
     * @return GPIO pin.
     */
    virtual uint8_t pin() const noexcept = 0;

    /**
     * @brief Get GPIO mode.
     *
     * @return Configured GPIO mode.
     */
    virtual Mode mode() const noexcept = 0;

    /**
     * @brief Set GPIO state.
     *
     * @param[in] state GPIO state (true = high, false = low).
     */
    virtual void write(const bool state) noexcept = 0;

    /**
     * @brief Read GPIO state.
     *
     * @return True if high, false if low.
     */
    virtual bool read() const noexcept = 0;

    /**
     * @brief Toggle GPIO state.
     */
    virtual void toggle() noexcept = 0;

    /**
     * @brief Check if a GPIO event has occurred since last read.
     * 
     * @return True on event, false otherwise.
     */
    virtual bool event() noexcept = 0;
};
} // namespace gpio
