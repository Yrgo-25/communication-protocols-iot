/**
 * @brief Frame definitions.
 */
#pragma once

#include <cstddef>
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

/**
 * @brief Frame offset structure.
 */
struct Offset
{
    static constexpr std::size_t Sof{0U};  ///< Start-of-Field offset.
    static constexpr std::size_t Len{2U};  ///< Payload length offset.
    static constexpr std::size_t Type{3U}; ///< Frame type offset.
    static constexpr std::size_t Dst{4U};  ///< Destination address offset.
    static constexpr std::size_t Src{5U};  ///< Source address offset.
    static constexpr std::size_t Seq{6U};  ///< Sequence number offset.
    static constexpr std::size_t Data{8U}; ///< Data (payload) offset.

    /**
     * @brief Get checksum offset based on the data length.
     * 
     * @param[in] dataLen Data (payload) length.
     * 
     * @return The checksum offset.
     */
    static constexpr std::size_t chk(const std::size_t dataLen) noexcept
    {
        return Data+ dataLen;
    }
};

/** Header length. */
constexpr std::size_t HeaderLen{8U};

/** Footer length. */
constexpr std::size_t FooterLen{2U};

/** Maximal payload length. */
constexpr std::size_t MaxPayloadLen{32U};

/** Minimum frame length. */
constexpr std::size_t MinFrameLen{HeaderLen + FooterLen};

/** Maximum frame length. */
constexpr std::size_t MaxFrameLen{MinFrameLen + MaxPayloadLen};

/** Start-of-Frame. */
constexpr std::uint16_t Sof{0xA5F7U};

} // namespace comm::frame
