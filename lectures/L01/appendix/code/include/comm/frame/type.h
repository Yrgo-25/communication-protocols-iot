/**
 * @brief Frame types.
 */
#pragma once

#include <cstdint>

namespace comm::frame
{
/**
 * @brief Enumeration of frame types.
 */
enum class Type : std::uint8_t
{
    ///< Ping frame.
    ///< Pong frame.
    ///< Status request frame.
    ///< Status response frame.
    ///< Unknown frame.
};
} // namespace comm::frame
