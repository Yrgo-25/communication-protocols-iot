/**
 * @brief Simple GPIO wrapper for Arduino boards.
 */
#pragma once

#include <stdint.h>

#include "gpio_interface.h"

namespace gpio
{
/**
 * @brief Simple GPIO wrapper for Arduino boards.
 *
 *        This class is non-copyable and non-movable.
 */
class Arduino final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] pin GPIO pin.
     * @param[in] mode GPIO mode.
     */
    explicit Arduino(uint8_t pin, Mode mode) noexcept;

        /**
     * @brief Destructor.
     */
    ~Arduino() noexcept override;

    /**
     * @brief Check if the GPIO is initialized.
     *
     * @return True if initialized, false othewise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Get GPIO pin.
     *
     * @return GPIO pin.
     */
    uint8_t pin() const noexcept override;

    /**
     * @brief Get GPIO mode.
     *
     * @return Configured GPIO mode.
     */
    Mode mode() const noexcept override;

    /**
     * @brief Set GPIO state.
     *
     * @param[in] state GPIO state (true = high, false = low).
     */
    void write(const bool state) noexcept override;

    /**
     * @brief Read GPIO state.
     *
     * @return True if high, false if low.
     */
    bool read() const noexcept override;

    /**
     * @brief Toggle GPIO state.
     */
    void toggle() noexcept override;

    /**
     * @brief Check if a GPIO event has occurred since last read.
     * 
     * @return True on event, false otherwise.
     */
    bool event() noexcept override;

    Arduino()                          = delete; // No default constructor.
    Arduino(const Arduino&)            = delete; // No copy constructor.
    Arduino(Arduino&&)                 = delete; // No move constructor.
    Arduino& operator=(const Arduino&) = delete; // No copy assignment..
    Arduino& operator=(Arduino&&)      = delete; // No move assignment. 
    
private:
    /** GPIO pin. */
    const uint8_t myPin;

    /** GPIO mode. */
    const Mode myMode;

    /** GPIO state. */
    bool myState;
};
} // namespace
