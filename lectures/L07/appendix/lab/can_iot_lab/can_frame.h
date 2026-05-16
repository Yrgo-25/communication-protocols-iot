/**
 * @brief CAN frame implementation.
 */
#pragma once

#include <stdint.h>

namespace can
{
/**
 * @brief CAN frame structure.
 */
struct Frame
{
    /** Max DLC (payload length). */
    static constexpr uint8_t MaxDlc{8U};

    /** CAN frame ID (11-bit when using standard CAN). */
    uint32_t id;

    /** Payload (0-8 bytes). */
    uint8_t data[MaxDlc];

    /** Data length (0-8). */
    uint8_t dlc;

    /**
     * @brief Constructor.
     * 
     * @param[in] id CAN ID (default = 0U).
     */
    explicit Frame(const uint32_t id = 0U) noexcept
        : id{id}
        , data{}
        , dlc{}
    {}

    /**
     * @brief Constructor.
     * 
     * @param[in] id CAN ID.
     * @param[in] buf Buffer holding the payload (0-8 bytes).
     * @param[in] bufLen Buffer length.
     */
    explicit Frame(const uint32_t id, const uint8_t* buf, const uint8_t bufLen) noexcept
        : Frame{id}
    {
        // Copy provided data (0-8 bytes) if present.
        if (nullptr == buf) { return; }
        dlc = MaxDlc < bufLen ? MaxDlc : bufLen;
        for (uint8_t i{}; i < dlc; ++i) { data[i] = buf[i]; }
    }
};
} // namespace can
