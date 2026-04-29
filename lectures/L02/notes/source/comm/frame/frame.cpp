/**
 * @brief Frame implementation details.
 */
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "comm/frame/def.h"
#include "comm/frame/frame.h"

namespace comm::frame
{
namespace
{
/**
 * @brief Frame header structure.
 */
struct FrameHeader
{
    /** Start-Of-Frame. */
    std::uint16_t sof{};

    /** Payload length. */
    std::uint8_t len{};

    /** Frame type. */
    Type type{};

    /** Destination address. */
    std::uint8_t dst{};

    /** Source address. */
    std::uint8_t src{};

    /** Sequence number. */
    std::uint16_t seq{};
};

// -----------------------------------------------------------------------------
template<typename T>
constexpr void writeToBuf(std::uint8_t* buf, const std::size_t offset, const T value) noexcept
{
    static_assert(std::is_unsigned<T>::value, "Type T must be unsigned!");
    constexpr std::size_t len{sizeof(T)};
    constexpr std::size_t last{len - 1U};

    for (std::size_t i{}; i < len; ++i)
    {
        const std::size_t shift{8U * i};
        const std::size_t index{last - i};
        buf[offset + index] = static_cast<std::uint8_t>(value >> shift);
    }
}

// -----------------------------------------------------------------------------
template<typename T = std::uint8_t>
constexpr T readFromBuf(const std::uint8_t* buf, const std::size_t offset) noexcept
{
    static_assert(std::is_unsigned<T>::value, "Type T must be unsigned!");
    constexpr std::size_t len{sizeof(T)};
    constexpr std::size_t last{len - 1U};
    T value{};

    for (std::size_t i{}; i < len; ++i)
    {
        const std::size_t shift{8U * i};
        const std::size_t index{last - i};
        value |= static_cast<T>(buf[offset + index]) << shift;
    }
    return value;
}

// -----------------------------------------------------------------------------
std::uint16_t computeChecksum(const std::uint8_t* buf, const std::size_t chkOffset) noexcept
{
    std::uint16_t checksum{};
    for (std::size_t i{}; i < chkOffset; ++i)
    {
        checksum += buf[i];
    }
    return checksum;
}

// -----------------------------------------------------------------------------
constexpr bool isFrameTypeValid(const Type type) noexcept
{
    constexpr std::uint8_t unknownFrame{static_cast<std::uint8_t>(Type::Unknown)};
    return unknownFrame > static_cast<std::uint8_t>(type);
}
} // namespace

// -----------------------------------------------------------------------------
std::size_t Frame::serialize(std::uint8_t* buf, const std::size_t bufLen) const noexcept
{
    // Check buffer, return 0 if invalid or too small.
    if (nullptr == buf) { return 0U; }
    const std::size_t frameLen{MinFrameLen + payloadLen};
    if (frameLen > bufLen) { return 0U; }

    // Serialize header.
    writeToBuf(buf, Offset::Sof, Sof);
    writeToBuf(buf, Offset::Len, payloadLen);
    writeToBuf(buf, Offset::Type, static_cast<std::uint8_t>(type));
    writeToBuf(buf, Offset::Dst, dstAddr);
    writeToBuf(buf, Offset::Src, srcAddr);
    writeToBuf(buf, Offset::Seq, seqNr);

    // Serialize payload (if present).
    for (std::size_t i{}; i < payloadLen; ++i)
    {
        buf[Offset::Data + i] = payload[i];
    }

    // Compute and serialize checksum.
    const std::size_t chkOffset{Offset::chk(payloadLen)};
    const std::uint16_t chk{computeChecksum(buf, chkOffset)};
    writeToBuf(buf, chkOffset, chk);

    // Return the number of serialized bytes.
    return frameLen;
}

// -----------------------------------------------------------------------------
bool Frame::deserialize(const std::uint8_t* buf, const std::size_t bufLen) noexcept
{
    // Check buffer, return false if invalid or too small.
    if (nullptr == buf) { return false; }
    if (MinFrameLen > bufLen) { return false; }

    FrameHeader header{};

    // Check SOF, return false if incorrect.
    header.sof = readFromBuf<std::uint16_t>(buf, Offset::Sof);
    if (Sof != header.sof) { return false; }
    
    // Parse payload length, return false if the frame is too small (with the payload included).
    header.len = readFromBuf(buf, Offset::Len);
    const std::size_t frameLen{MinFrameLen + header.len};
    if (frameLen > bufLen) { return false; }

    // Parse frame type, return false if invalid.
    header.type = static_cast<Type>(readFromBuf(buf, Offset::Type));
    if (!isFrameTypeValid(header.type)) { return false; }

    // Parse destination and source address.
    header.dst = readFromBuf(buf, Offset::Dst);
    header.src = readFromBuf(buf, Offset::Src);

    // Parse sequence number.
    header.seq = readFromBuf<std::uint16_t>(buf, Offset::Seq);
    
    // Compute the checksum.
    const std::size_t chkOffset{Offset::chk(header.len)};
    const std::uint16_t expectedChk{computeChecksum(buf, chkOffset)};

    // Parse checksum, return false if invalid (mismatch with the computed value).
    const std::uint16_t actualChk{readFromBuf<std::uint16_t>(buf, chkOffset)};
    if (actualChk != expectedChk) { return false; }

    // Copy parsed data to the frame.
    payloadLen = header.len;
    type       = header.type;
    dstAddr    = header.dst;
    srcAddr    = header.src;
    seqNr      = header.seq;

    // Copy payload if present.
    for (std::uint8_t i{}; i < header.len; ++i)
    {
        payload[i] = buf[Offset::Data + i];
    }
    // Return true to indicate success.
    return true;
}
} // namespace comm::frame
