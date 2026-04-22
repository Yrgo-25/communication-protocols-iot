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
} // namespace comm::frame
