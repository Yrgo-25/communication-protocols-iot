/**
 * @brief System logic implementation.
 */
#pragma once

#include <stdint.h>

/** CAN driver interface. */
namespace can::driver { class Interface; }

/** GPIO interface. */
namespace gpio { class Interface; }

namespace sys
{
/**
 * @brief System configuration structure.
 */
struct Config
{
    /** Serial baud rate in bps (bits per second). */
    const uint32_t baudRate_bps;
    
    /** CAN ID for transmitting button 1 state. */
    const uint32_t button1StateId;

    /** CAN ID for transmitting button 2 state. */
    const uint32_t button2StateId;
};

/**
 * @brief System logic implementation.
 * 
 *        This class is non-copyable and non-movable.
 */
class Logic final
{
public:
    /**
     * @brief Constructor.
     * 
     * @param[in] canDriver CAN driver for communication.
     * @param[in] led1 First LED.
     * @param[in] led2 Second LED.
     * @param[in] button1 First button.
     * @param[in] button2 Second button.
     * @param[in] config System configuration.
     */
    explicit Logic(can::driver::Interface& canDriver, gpio::Interface& led1, gpio::Interface& led2,
                   gpio::Interface& button1, gpio::Interface& button2,
                   const Config& config) noexcept;

    /** 
     * @brief Destructor.
     */
    ~Logic() noexcept = default;

    /**
     * @brief Initialize logic.
     * 
     * @return True on success, false on failure.
     */
    bool init() noexcept;

    /**
     * @brief Run logic loop once.
     */
    void run() noexcept;

    Logic()                        = delete; // No default constructor.
    Logic(const Logic&)            = delete; // No copy constructor.
    Logic(Logic&&)                 = delete; // No move constructor.
    Logic& operator=(const Logic&) = delete; // No copy assignment.
    Logic& operator=(Logic&&)      = delete; // No move assignment.

private:
    bool handleButtonEvent(gpio::Interface& button, uint32_t canId) noexcept;
    void readIncomingData() noexcept;

    /** CAN driver. */
    can::driver::Interface& myCanDriver;

    /** First LED. */
    gpio::Interface& myLed1;
    
    /** Second LED. */
    gpio::Interface& myLed2;

    /** First button. */
    gpio::Interface& myButton1;

    /** Second button. */
    gpio::Interface& myButton2;

    /** System configuration. */
    const Config& myConfig;
};
} // namespace sys
