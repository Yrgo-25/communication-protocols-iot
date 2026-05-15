/**
 * @brief CAN driver interface.
 */
#pragma once

#include <stdint.h>

/** CAN frame structure. */
namespace can { struct Frame; }

namespace can::driver
{
/**
 * @brief CAN driver interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Check if the driver is initialized.
     *
     * @return True if initialized, false otherwise.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Initialize CAN driver.
     *
     * @return True on success, false on failure.
     */
    virtual bool init() noexcept = 0;

    /**
     * @brief Check if data is available.
     *
     * @return True if available, false otherwise.
     */
    virtual bool isDataAvailable() const noexcept = 0;

    /**
     * @brief Receive data.
     *
     * @param[out] frame Frame holding received data.
     *
     * @return True if any data was received, false otherwise.
     */
    virtual bool receive(Frame& frame) noexcept = 0;

    /**
     * @brief Send data.
     *
     * @param[in] frame Frame to send.
     *
     * @return Number of transmitted bytes.
     */
    virtual uint8_t send(const Frame& frame) noexcept = 0;
};
} // namespace can::driver
