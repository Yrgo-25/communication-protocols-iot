/**
 * @brief Frame implementation details.
 */
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "comm/frame/def.h"
#include "comm/frame/frame.h"

namespace comm::frame
{
namespace
{
/** Byte shift value for bit operations. */
constexpr std::size_t ByteShift{8U};

// -----------------------------------------------------------------------------
constexpr void write(std::uint8_t* buf, const std::size_t offset, 
    const std::uint8_t value) noexcept
{
    buf[offset] = value;
}

// -----------------------------------------------------------------------------
constexpr void write(std::uint8_t* buf, const std::size_t offset, 
    const std::uint16_t value) noexcept
{
    buf[offset]      = static_cast<std::uint8_t>(value >> ByteShift);   
    buf[offset + 1U] = static_cast<std::uint8_t>(value);
}

// -----------------------------------------------------------------------------
constexpr void write(std::uint8_t* buf, const std::size_t offset, const std::uint8_t* payload,
                     const std::uint8_t payloadLen) noexcept
{
    // Copy the payload to the buffer byte by byte.
    for (std::uint8_t i{}; i < payloadLen; ++i)
    {
        buf[offset + i] = payload[i];
    }
}

// -----------------------------------------------------------------------------
constexpr std::uint8_t readU8(const std::uint8_t* buf, const std::size_t offset) noexcept
{
    return buf[offset];
}

// -----------------------------------------------------------------------------
constexpr std::uint16_t readU16(const std::uint8_t* buf, const std::size_t offset) noexcept
{
    std::uint16_t value{};

    // Read MSB, place it at the upper byte of 'value'.
    value |= (static_cast<std::uint16_t>(buf[offset]) << ByteShift);

    // Resst LSB, please it at the lower byte of value.
    value |= static_cast<std::uint16_t>(buf[offset + 1U]);
    return value;
}

// -----------------------------------------------------------------------------
constexpr std::uint16_t computeChecksum(const std::uint8_t* buf, 
                                        const std::size_t chkOffset) noexcept
{
    std::uint16_t checksum{};

    // Add the value of each byte in the buffer via a for loop.
    for (std::size_t i{}; i < chkOffset; ++i)
    {
        checksum += buf[i];
    }
    // Return the computed checksum.
    return checksum;
}
} // namespace

// -----------------------------------------------------------------------------
std::size_t Frame::serialize(std::uint8_t* buf, std::size_t bufLen) const noexcept
{
    // Check the buffer, return 0 if invalid.
    if (nullptr == buf) { return 0U; }

    // Compute our frame length, return 0 if the buffer is too small.
    const std::size_t frameLen{MinFrameLen + payloadLen};
    if (frameLen > bufLen) { return 0U; }

    // Write header (SOF => LEN => TYPE => DST => SRC => SEQ).
    write(buf, Offset::Sof, Sof);
    write(buf, Offset::Len, payloadLen);
    write(buf, Offset::Type, static_cast<std::uint8_t>(type));
    write(buf, Offset::Dst, dstAddr);
    write(buf, Offset::Src, srcAddr);
    write(buf, Offset::Seq, seqNr);

    // Write our payload.
    write(buf, Offset::Data, payload, payloadLen);

    // Compute the checksum offset.
    const std::size_t chkOffset{Offset::chk(payloadLen)};

    // Compute checksum.
    const std::uint16_t checksum{computeChecksum(buf, chkOffset)};

    // Write the checksum, then return the number of serialized bytes, i.e. our frame length.
    write(buf, chkOffset, checksum);
    return frameLen;
}

// -----------------------------------------------------------------------------
bool Frame::deserialize(const std::uint8_t* buf, std::size_t bufLen) noexcept
{
    // Check the buffer, return false if invalid.
    if (nullptr == buf) { return false; }

    // Check the buffer length, return false if too small for even the smallest frame.
    if (MinFrameLen > bufLen) { return false; }

    // Read the SOF value, return false if incorrect.
    const std::uint16_t retrievedSof{readU16(buf, Offset::Sof)};

    if (Sof != retrievedSof) 
    {
        std::printf("Incorrect SOF!\n"); 
        return false; 
    }

    // Read the payload length, return false if too big (> MaxPayloadLen).
    const std::uint8_t retrievedLen{readU8(buf, Offset::Len)};

    if (MaxPayloadLen < retrievedLen)
    {
        std::printf("Invalid payload length %u!\n", retrievedLen);
        return false;
    }

    // Read the frame type, return false if invalid (compare with Type::Unknown).
    const std::uint8_t retrievedType{readU8(buf, Offset::Type)};

    if (static_cast<std::uint8_t>(Type::Unknown) <= retrievedType)
    {
        std::printf("Invalid frame type (ID = %u)!\n", retrievedType);
        return false;
    }

    // Retrieve DST, SRC, and SEQ number.
    const std::uint8_t retrievedDst{readU8(buf, Offset::Dst)};
    const std::uint8_t retrievedSrc{readU8(buf, Offset::Src)};
    const std::uint16_t retrievedSeq{readU16(buf, Offset::Seq)};

    // Any constraints regarding DST and SRC addresses would be utilized now.

    // Compute the checksum offset based on the retrieved payload length.
    const std::size_t chkOffset{Offset::chk(retrievedLen)};

    // Compute and print the actual checksum.
    const std::uint16_t actualChecksum{computeChecksum(buf, chkOffset)};
    std::printf("Actual checksum: %02X!\n", actualChecksum);

    // Retrieve and print the checksum from the buffer.
    const std::uint16_t retrievedChecksum{readU16(buf, chkOffset)};
    std::printf("Retrieved checksum: %02X!\n", retrievedChecksum);

    // Verify match with the checksum in the buffer, return false on mismatch.
    if (actualChecksum != retrievedChecksum)
    {
        std::printf("Invalid checksum: actual = %02X, retrieved = %02X!\n",
            actualChecksum, retrievedChecksum);
        return false;
    }

    // Last but not least, store retrieved parameters.
    payloadLen = retrievedLen;
    type       = static_cast<Type>(retrievedType);
    dstAddr    = retrievedDst;
    srcAddr    = retrievedSrc;
    seqNr      = retrievedSeq;

    // Copy the payload from the buffer.
    for (std::uint8_t i{}; i < payloadLen; ++i)
    {
        payload[i] = buf[Offset::Data + i];
    }
    // Return true to indicate success.
    return true;
}
} // namespace comm::frame
