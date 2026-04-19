/**
 * @brief Frame implementation.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "comm/frame/type.h"

namespace comm::frame
{
/**
 * @brief Frame structure.
 */
struct Frame
{
    /** Header length. */
    static constexpr std::size_t HeaderLen{8U};

    /** Footer length. */
    static constexpr std::size_t FooterLen{2U};

    /** Maximal payload length. */
    static constexpr std::size_t MaxPayloadLen{32U};

    /** Maximal frame length. */
    static constexpr std::size_t MaxFrameLen{HeaderLen + FooterLen + MaxPayloadLen};

    /** Start-of-Frame. */
    static constexpr std::uint16_t Sof{0xA5F7U};

    /** Payload buffer. */
    std::uint8_t payload[MaxPayloadLen]{};

    /** Sequence. */
    std::uint16_t seqNr{};

    /** Destination address. */
    std::uint8_t dstAddr{};

    /** Source address. */
    std::uint8_t srcAddr{};

    /** Payload length. */
    std::uint8_t payloadLen{};

    /** Frame type. */
    Type type{Type::Unknown};

    /**
     * @brief Serialize frame.
     * 
     * @param[out] buf Buffer to serialize.
     * @param[in] bufLen Length of the buffer.
     * 
     * @return The number of serialized bytes.
     */
    std::size_t serialize(std::uint8_t* buf, std::size_t bufLen) const noexcept;

    /**
     * @brief Deserialize frame.
     * 
     * @param[in] buf Buffer to deserialize.
     * @param[in] bufLen Length of the buffer.
     * 
     * @return True on success, false on failure.
     */
    bool deserialize(const std::uint8_t* buf, std::size_t bufLen) noexcept;
};

/**
 * @brief Frame offsets.
 */
struct Offset
{
    static constexpr std::size_t Sof{0U};     ///< Start-of-Field offset.
    static constexpr std::size_t Len{2U};     ///< Payload length offset.
    static constexpr std::size_t Type{3U};    ///< Frame type offset.
    static constexpr std::size_t Dst{4U};     ///< Destination address offset.
    static constexpr std::size_t Src{5U};     ///< Source address offset.
    static constexpr std::size_t Seq{6U};     ///< Sequence number offset.
    static constexpr std::size_t Payload{8U}; ///< Payload offset.

    /**
     * @brief Get checksum offset based on the payload length.
     * 
     * @param[in] payloadLen Payload length.
     * 
     * @return The checksum offset.
     */
    static constexpr std::size_t chk(const std::size_t payloadLen) noexcept
    {
        return Payload + payloadLen;
    }
};
} // namespace comm::frame
