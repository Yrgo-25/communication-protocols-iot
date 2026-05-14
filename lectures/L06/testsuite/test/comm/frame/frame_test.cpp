/**
 * @brief Unit tests for comm::frame::Frame.
 */
#include <cstdint>

#include <gtest/gtest.h>

#include "comm/frame/def.h"
#include "comm/frame/frame.h"

using namespace comm::frame;

namespace
{
// -----------------------------------------------------------------------------
constexpr std::uint8_t upperByte(const std::uint16_t value) noexcept
{
    constexpr std::size_t shift{8U};
    return static_cast<std::uint8_t>(value >> shift);
}

// -----------------------------------------------------------------------------
constexpr std::uint8_t lowerByte(const std::uint16_t value) noexcept
{
    return static_cast<std::uint8_t>(value);
}

// -----------------------------------------------------------------------------
constexpr std::size_t frameLen(const Frame& frame) noexcept
{
    return MinFrameLen + frame.payloadLen;
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
void matchFrameContent(const Frame& frame, const std::uint8_t* buf) noexcept
{
    // Except SOF to be correct.
    EXPECT_EQ(buf[Offset::Sof], upperByte(Sof));
    EXPECT_EQ(buf[Offset::Sof + 1U], lowerByte(Sof));

    // Expect the payload length and frame type to match.
    EXPECT_EQ(buf[Offset::Len], frame.payloadLen);
    EXPECT_EQ(buf[Offset::Type], static_cast<std::uint8_t>(frame.type));

    // Expect the destination and source addresses to match.
    EXPECT_EQ(buf[Offset::Dst], frame.dstAddr);
    EXPECT_EQ(buf[Offset::Src], frame.srcAddr);

    // Expect the sequence number to match.
    EXPECT_EQ(buf[Offset::Seq], upperByte(frame.seqNr));
    EXPECT_EQ(buf[Offset::Seq + 1U], lowerByte(frame.seqNr));

    // Expect the payload to match.
    for (std::uint8_t i{}; i < frame.payloadLen; ++i)
    {
        EXPECT_EQ(buf[Offset::Data + i], frame.payload[i]);
    }

    // Compute the checksum, expect the computed value to match the checksum in the buffer.
    const std::size_t chkOffset{Offset::chk(frame.payloadLen)};
    const std::uint16_t chk{computeChecksum(buf, chkOffset)};
    EXPECT_EQ(buf[chkOffset], upperByte(chk));
    EXPECT_EQ(buf[chkOffset + 1U], lowerByte(chk));
}

// -----------------------------------------------------------------------------
void matchFrames(const Frame& frame1, const Frame& frame2) noexcept
{
    // Expect every member variable to be identical.
    EXPECT_EQ(frame1.payloadLen, frame2.payloadLen);
    EXPECT_EQ(frame1.type, frame2.type);
    EXPECT_EQ(frame1.dstAddr, frame2.dstAddr);
    EXPECT_EQ(frame1.srcAddr, frame2.srcAddr);
    EXPECT_EQ(frame1.seqNr, frame2.seqNr);
    
    for (std::uint8_t i{}; i < frame1.payloadLen; ++i)
    {
        EXPECT_EQ(frame1.payload[i], frame2.payload[i]);
    }
}

/**
 * @brief Happy path test.
 * 
 *        Create two valid frames with different payloads.
 *        Expect serialization and deserialization to succeed.
 */
TEST(FrameTest, HappyPath)
{
    constexpr std::uint8_t dstAddr{0x03U};
    constexpr std::uint8_t srcAddr{0x01U};
    constexpr std::uint16_t seqNr{0x20U};
    constexpr std::size_t bufLen{20U};

    // Test case 1 - Serialize and deserialize a valid ping frame.
    {
        // Create a ping frame.
        Frame txFrame{};
        txFrame.type    = Type::Ping;
        txFrame.dstAddr = dstAddr;
        txFrame.srcAddr = srcAddr;
        txFrame.seqNr   = seqNr;

        // Serialize the frame, expect success.
        std::uint8_t txBuf[bufLen]{};
        const std::size_t bytesSerialized{txFrame.serialize(txBuf, bufLen)};

        // Expect the entire frame to be serialized.
        EXPECT_EQ(frameLen(txFrame), bytesSerialized);

        // Investigate the frame buffer, expect the content to match the frame.
        matchFrameContent(txFrame, txBuf);

        // Deserialize the frame, expect success.
        Frame rxFrame{};
        EXPECT_TRUE(rxFrame.deserialize(txBuf, bufLen));

        // Match the frames, expect them to be identical.
        matchFrames(txFrame, rxFrame);
    }

    //! @todo Implement this test!
    // Test case 2 - Serialize and deserialize a valid status response frame.
    {
        // Create a status response frame, send sensor value 0x0016 (2 bytes) as payload.
    
        // Serialize the frame, expect success.

        // Expect the entire frame to be serialized.

        // Investigate the frame buffer, expect the content to match the frame.

        // Deserialize the frame, expect success.

        // Match the frames, expect them to be identical.
    }
}

/**
 * @brief Invalid frame test.
 * 
 *        Simulate bit flips and missing bytes in the frame content.
 *        Expect deserialization to fail due to checksum mismatch.
 */
TEST(FrameTest, InvalidFrame)
{
    constexpr std::uint8_t dstAddr{0x03U};
    constexpr std::uint8_t srcAddr{0x01U};
    constexpr std::uint16_t seqNr{0x20U};
    constexpr std::size_t bufLen{20U};

    // Test case 1 - Serialize and deserialize a ping frame with a flipped bit.
    {
        // Create a ping frame.
        Frame txFrame{};
        txFrame.type    = Type::Ping;
        txFrame.dstAddr = dstAddr;
        txFrame.srcAddr = srcAddr;
        txFrame.seqNr   = seqNr;

        // Serialize the frame, expect success.
        std::uint8_t txBuf[bufLen]{};
        const std::size_t bytesSerialized{txFrame.serialize(txBuf, bufLen)};

        // Expect the entire frame to be serialized.
        EXPECT_EQ(frameLen(txFrame), bytesSerialized);

        // Flip a bit in a register.
        constexpr std::uint8_t flippedIdx{5U};
        constexpr std::uint8_t flippedBit{3U};
        txBuf[flippedIdx] ^= (1U << flippedBit);

        // Deserialize the frame, expect failure due to checksum mismatch.
        Frame rxFrame{};
        EXPECT_FALSE(rxFrame.deserialize(txBuf, bufLen));
    }

    // Test case 2 - Serialize and deserialize a status response frame with a missing byte.
    {
        // Create a status response frame, send value 0xDEADBEEF (2 bytes) as payload.
        constexpr std::uint32_t payload{0xDEADBEEFU};
        constexpr std::uint8_t payloadLen{static_cast<std::uint8_t>(sizeof(payload))};
        
        Frame txFrame{};
        txFrame.payloadLen = payloadLen;
        txFrame.type       = Type::StatusResponse;
        txFrame.dstAddr    = dstAddr;
        txFrame.srcAddr    = srcAddr;
        txFrame.seqNr      = seqNr;
    
        // Serialize the frame, expect success.
        std::uint8_t txBuf[bufLen]{};
        const std::size_t bytesSerialized{txFrame.serialize(txBuf, bufLen)};

        // Expect the entire frame to be serialized.
        EXPECT_EQ(frameLen(txFrame), bytesSerialized);

        // Remove one byte from the buffer.
        constexpr std::size_t removedByteIdx{4U};

        for (std::size_t i{removedByteIdx}; i < frameLen(txFrame); ++i)
        {
            txBuf[i] = txBuf[i + 1U];
        }
        txBuf[frameLen(txFrame) - 1U] = 0U;

        // Deserialize the frame, expect failure due to checksum mismatch.
        Frame rxFrame{};
        EXPECT_FALSE(rxFrame.deserialize(txBuf, bufLen));
    }
}
} // namespace
