/**
 * @brief Frame parser implementation details.
 */
#include <cstdint>

#include "comm/frame/def.h"
#include "comm/frame/frame.h"
#include "comm/frame/parser.h"

namespace comm::frame
{
namespace
{
/** Byte shift parameter for bit operations. */
constexpr std::size_t ByteShift{8U};

// -----------------------------------------------------------------------------
constexpr std::uint8_t readByte(const std::uint8_t* buf, const std::size_t offset) noexcept
{
    return buf[offset];
}

// -----------------------------------------------------------------------------
constexpr std::uint16_t readWord(const std::uint8_t* buf, const std::size_t offset) noexcept
{
    return (static_cast<std::uint16_t>(buf[offset]) << ByteShift) | 
           (static_cast<std::uint16_t>(buf[offset + 1U]));
}

// -----------------------------------------------------------------------------
constexpr std::uint8_t highByte(const std::uint16_t value) noexcept
{
    return static_cast<std::uint8_t>(value >> ByteShift);
}

// -----------------------------------------------------------------------------
constexpr std::uint8_t lowByte(const std::uint16_t value) noexcept
{
    return static_cast<std::uint8_t>(value);
}
} // namespace

// -----------------------------------------------------------------------------
Parser::Parser() noexcept
    : myBuf{}
    , myParsedBytes{}
    , myState{State::WaitForSof1}
{}

// -----------------------------------------------------------------------------
bool Parser::isFrameReady() const noexcept { return State::Ready == myState; }

// -----------------------------------------------------------------------------
bool Parser::processByte(std::uint8_t byte) noexcept
{
    // Check the current state.
    switch (myState)
    {
        case State::WaitForSof1:
        {
            // Look for high byte of SOF, report error on mismatch.
            const std::uint8_t sof1{highByte(Sof)};
            if (sof1 != byte) { return reportParseError(); }

            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::WaitForSof2;
            break;
        }
        case State::WaitForSof2:
        {
            // Look for low byte of SOF, report error on mismatch.
            const std::uint8_t sof2{lowByte(Sof)};
            if (sof2 != byte) { return reportParseError(); }

            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::WaitForLen;
            break;
        }
        case State::WaitForLen:
        {
            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::WaitForType;
            break;
        }
        case State::WaitForType:
        {
            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::WaitForDst;
            break;
        }
        case State::WaitForDst:
        {
            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::WaitForSrc;
            break;
        }
        case State::WaitForSrc:
        {
            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::WaitForSeq1;
            break;
        }
        case State::WaitForSeq1:
        {
            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::WaitForSeq2;
            break;
        }
        case State::WaitForSeq2:
        {
            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::WaitForPayload;
            break;
        }
        case State::WaitForPayload:
        {
            // Compute the expected and actual payload length.
            const std::size_t expectedLen{static_cast<std::size_t>(myBuf[Offset::Len])};
            const std::size_t actualLen{static_cast<std::size_t>(myParsedBytes) - HeaderLen};

            // Go immediately to next state if all payload bytes have been parsed.
            if (expectedLen <= actualLen) { myState = State::WaitForChk1; }
            else
            {
                // Else parse the next payload byte, stay in the current state.
                myBuf[myParsedBytes++] = byte;
                break;
            }
        }
        case State::WaitForChk1:
        {
            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::WaitForChk2;
            break;
        }
        case State::WaitForChk2:
        {
            // Parse the byte, go to next state.
            myBuf[myParsedBytes++] = byte;
            myState                = State::Ready;
            break;
        }
        case State::Ready:
        {
            // Frame already complete, skip processing byte.
            return false;
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
bool Parser::extractFrame(Frame& frame) const noexcept
{
    // Check if frame is ready to be extracted, return false if not.
    if (!isFrameReady()) { return false; }

    // Copy header.
    frame.payloadLen = readByte(myBuf, Offset::Len);
    frame.type       = static_cast<Type>(readByte(myBuf, Offset::Type));
    frame.dstAddr    = readByte(myBuf, Offset::Dst);
    frame.srcAddr    = readByte(myBuf, Offset::Src);
    frame.seqNr      = readWord(myBuf, Offset::Seq);
    
    // Copy payload.
    for (std::uint8_t i{}; i < frame.payloadLen; ++i)
    {
        frame.payload[i] = readByte(myBuf, Offset::Data + i);
    }
    // Return true to indicate success.
    return true;
}

// -----------------------------------------------------------------------------
void Parser::reset() noexcept
{
    for (auto& byte : myBuf) { byte = 0U; }
    myParsedBytes = 0U;
    myState       = State::WaitForSof1;
}

// -----------------------------------------------------------------------------
bool Parser::reportParseError() noexcept
{
    reset();
    return false;
}
} // namespace comm::frame
