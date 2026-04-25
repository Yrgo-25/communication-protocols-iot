/**
 * @brief Frame implementation.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "comm/frame/def.h"

namespace comm::frame
{
/**
 * @brief Frame structure.
 */
struct Frame
{
    /** Payload buffer. */

    /** Sequence number. */

    /** Destination address. */

    /** Source address. */

    /** Payload length. */

    /** Frame type. */

    /**
     * @brief Serialize frame.
     * 
     * @param[out] buf Buffer to serialize.
     * @param[in] bufLen Length of the buffer.
     * 
     * @return The number of serialized bytes.
     */

    /**
     * @brief Deserialize frame.
     * 
     * @param[in] buf Buffer to deserialize.
     * @param[in] bufLen Length of the buffer.
     * 
     * @return True on success, false on failure.
     */
};
} // namespace comm::frame
