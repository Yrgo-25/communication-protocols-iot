/**
 * @brief Frame parser test program.
 */
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "comm/frame/frame.h"
#include "comm/frame/handler.h"
#include "comm/frame/parser.h"
#include "comm/frame/type.h"

using namespace comm;

namespace
{
// -----------------------------------------------------------------------------
void printBytes(const std::uint8_t* bytes, const std::size_t byteCount) noexcept
{
    if ((nullptr == bytes) || (0U == byteCount)) { return; }
    const std::size_t last{byteCount - 1U};
    std::printf("[");
    for (std::size_t i{}; i < last; ++i) { std::printf("%02X, ", bytes[i]); }
    std::printf("%02X]\n", bytes[last]);
}

// -----------------------------------------------------------------------------
bool createPing(std::uint8_t* buf, std::size_t& bufLen, const std::uint8_t dst,
                const std::uint8_t src, const std::uint16_t seq) noexcept
{
    // Initialize ping frame structure.
    frame::Frame frame{};
    frame.type    = frame::Type::Ping;
    frame.dstAddr = dst;
    frame.srcAddr = src;
    frame.seqNr   = seq;

    // Serialize buffer, return false on failure.
    const std::size_t frameLen{frame.serialize(buf, bufLen)};
    if (0U == frameLen) { return false; }

    // Print the frame as bytes.
    std::printf("Ping frame (%zu bytes): ", frameLen);
    printBytes(buf, frameLen);

    // Store the number of serialized bytes.
    bufLen = frameLen;
    return true;
}

// -----------------------------------------------------------------------------
const char* typeStr(const comm::frame::Type type) noexcept
{
    switch (type)
    {
        case comm::frame::Type::Ping:
            return "Ping";
        case comm::frame::Type::Pong:
            return "Pong";
        case comm::frame::Type::StatusRequest:
            return "StatusRequest";
        case comm::frame::Type::StatusResponse:
            return "StatusResponse";
        default:
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
bool validatePong(const std::uint8_t* buf, const std::size_t bufLen) noexcept
{
    // Deserialize response data, return false on failure.
    frame::Frame frame{};
    if (!frame.deserialize(buf, bufLen)) { return false; }
    if (frame::Type::Pong != frame.type) { return false; }

    // Print the frame as bytes.
    std::printf("Pong frame (%zu bytes): ", bufLen);
    printBytes(buf, bufLen);

    // Print deserialized data.
    std::printf("Deserialized frame:\n");
    std::printf("\t- Frame type: %s\n", typeStr(frame.type));
    std::printf("\t- Destination address: %u\n", frame.dstAddr);
    std::printf("\t- Source address: %u\n", frame.srcAddr);
    std::printf("\t- Sequence: %u\n", frame.seqNr);
    
    // Print payload (if any).
    if (0U < frame.payloadLen)
    {
        std::printf("\t- Payload (%u) bytes: ", frame.payloadLen);
        printBytes(frame.payload, frame.payloadLen);
    }
    return true;
}
} // namespace 

/**
 * @brief Transmit ping and check response.
 * 
 * @return 0 on success, or -1 on failure.
 */
int main()
{
    // Create TX buffer.
    constexpr std::size_t bufLen{20U};
    std::uint8_t txBuf[bufLen]{};
    std::size_t txLen{sizeof(txBuf)};
    comm::frame::Frame txFrame{};

    // Serialize buffer with ping frame.
    {
        constexpr std::uint8_t pingDst{0x02U};
        constexpr std::uint8_t pingSrc{0x01U};
        constexpr std::uint8_t pingSeq{0x20U};
        if (!createPing(txBuf, txLen, pingDst, pingSrc, pingSeq)) { return -1;}
    }

    // Parse frame byte by byte, extract parsed frame.
    {
        // Create frame parser.
        comm::frame::Parser parser{};
        std::uint8_t byteIndex{};

        // Parse frame byte by byte, return -1 on failure.
        while (!parser.isFrameReady() && (bufLen > byteIndex))
        {
            if (!parser.processByte(txBuf[byteIndex++])) { return -1; }
        }
        // Extract parsed frame, return -1 on failure.
        if (!parser.extractFrame(txFrame)) { return -1; }
    }

    // Transmit frame, expect pong as response.
    {
        // Handle parsed frame, return -1 on failure.
        comm::frame::Frame rxFrame{};
        if (!comm::frame::handleFrame(txFrame, rxFrame)) { return -1; }
 
        // Serialize RX data.
        std::uint8_t rxBuf[bufLen]{};
        std::size_t rxLen{sizeof(rxBuf)};
        rxLen = rxFrame.serialize(rxBuf, rxLen);

        // Deserialize the response frame, terminate the program on failure.
        if (!validatePong(rxBuf, rxLen)) { return -1; }
    }
    return 0;
}
