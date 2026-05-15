/**
 * @brief CAN driver stub.
 */
#pragma once

#include <stdint.h>

#include "can_driver_interface.h"
#include "can_frame.h"

namespace can::driver
{
/**
 * @brief CAN driver stub.
 *
 *        This class is non-copyable and non-movable.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Constructor.
     *
     * @param[in] csPin SPI chip select pin.
     */
    explicit Stub(uint8_t csPin) noexcept
        : myInitialized{false}
        , myDataAvailable{false}
    {
        (void) (csPin);
    }

    /**
     * @brief Destructor.
     */
    ~Stub() noexcept override = default;

    /**
     * @brief Check if the driver is initialized.
     *
     * @return True if initialized, false otherwise.
     */
    bool isInitialized() const noexcept override { return myInitialized; }

    /**
     * @brief Initialize CAN driver.
     *
     * @return True on success, false on failure.
     */
    bool init() noexcept override 
    { 
        myInitialized = true;
        return true;
    }

    /**
     * @brief Check if data is available.
     *
     * @return True if available, false otherwise.
     */
    bool isDataAvailable() const noexcept override { return myDataAvailable; }

    /**
     * @brief Receive data.
     *
     * @param[out] frame Frame holding received data.
     *
     * @return True if any data was received, false otherwise.
     */
    bool receive(Frame& frame) noexcept override
    {
        // Copy RX buffer if data is available.
        if (isDataAvailable())
        {
            frame = myRxFrame;
            myDataAvailable = false;
            return true;
        }
        return false;
    }

    /**
     * @brief Send data.
     *
     * @param[in] frame Frame to send.
     *
     * @return Number of transmitted bytes.
     */
    uint8_t send(const Frame& frame) noexcept override
    {
        myRxFrame = frame;
        myDataAvailable = true;
        return frame.dlc;
    }

    Stub()                       = delete; // No default constructor.
    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** Received frame. */
    Frame myRxFrame;

    /** True if the CAN driver is initialized, false otherwise. */
    bool myInitialized;

    /** True if data is available, false otherwise. */
    bool myDataAvailable;
};
} // namespace can::driver
