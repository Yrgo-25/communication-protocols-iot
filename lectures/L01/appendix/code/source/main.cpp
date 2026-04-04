/**
 * @brief Frame test program.
 */
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include "frame.h"

using namespace comm;

namespace
{
// -----------------------------------------------------------------------------
void printBytes(const std::uint8_t* bytes, const std::size_t byteCount, 
                std::ostream& ostream = std::cout) noexcept
{
    if ((nullptr == bytes) || (0U == byteCount)) { return; }
    const std::size_t last{byteCount - 1U};
    ostream << "[";

    for (std::size_t i{}; i < last; ++i)
    {
        ostream << std::hex << static_cast<int>(bytes[i]) << ", ";
    }
    ostream << std::hex << static_cast<int>(bytes[last]) << "]\n";
}

// -----------------------------------------------------------------------------
bool createPing(std::uint8_t* buf, std::size_t& bufLen, const std::uint8_t dst,
                const std::uint8_t src, const std::uint16_t seq,
                std::ostream& ostream = std::cout) noexcept
{
    // Initialize ping frame structure.
    frame::Frame frame{};
    frame.type = frame::Type::Ping;
    frame.dstAddr  = dst;
    frame.srcAddr  = src;
    frame.seqNr  = seq;

    // Serialize buffer, return false on failure.
    const std::size_t frameLen{frame.serialize(buf, bufLen)};
    if (0U == frameLen) { return false; }

    // Print the frame as bytes.
    ostream << std::dec << "Ping frame (" << frameLen << " bytes): ";
    printBytes(buf, frameLen, ostream);

    // Store the number of serialized bytes.
    bufLen = frameLen;
    return true;
}

// -----------------------------------------------------------------------------
bool transmitPing(const std::uint8_t* txBuf, const std::size_t txBufLen,
                  std::uint8_t* rxBuf, std::size_t& rxBufLen) noexcept
{
    // Deserialize TX data, expect a ping frame.
    frame::Frame txFrame{};
    if (!txFrame.deserialize(txBuf, txBufLen)) { return false; }
    if (frame::Type::Ping != txFrame.type) { return false; }

    // Respond with a pong frame.
    frame::Frame rxFrame{};
    rxFrame.type = frame::Type::Pong;
    rxFrame.dstAddr  = txFrame.srcAddr;
    rxFrame.srcAddr  = txFrame.dstAddr;
    rxFrame.seqNr  = txFrame.seqNr;

    // Serialize RX data, return false on failure.
    const std::size_t rxLen{rxFrame.serialize(rxBuf, rxBufLen)};
    if ((0U == rxLen) || (frame::Type::Pong != rxFrame.type)) { return false; }

    // Store the number of serialized RX bytes.
    rxBufLen = rxLen;
    return true;
}

// -----------------------------------------------------------------------------
bool validatePong(const std::uint8_t* buf, const std::size_t bufLen,
                  std::ostream& ostream = std::cout) noexcept
{
    // Deserialize response data, return false on failure.
    frame::Frame frame{};
    if (!frame.deserialize(buf, bufLen)) { return false; }
    if (frame::Type::Pong != frame.type) { return false; }

    // Print the frame as bytes.
    ostream << std::dec << "Pong frame (" << bufLen << " bytes): ";
    printBytes(buf, bufLen, ostream);

    // Print deserialized data.
    ostream << "Deserialized frame:\n";
    ostream << "\t- Frame type ID: " << static_cast<int>(frame.type) << "\n";
    ostream << "\t- Destination address: " << static_cast<int>(frame.dstAddr) << "\n";
    ostream << "\t- Source address: " << static_cast<int>(frame.srcAddr) << "\n";
    ostream << "\t- Sequence: " << static_cast<int>(frame.seqNr) << "\n";
    
    // Print payload (if any).
    if (0U < frame.payloadLen)
    {
        ostream << "\t- Payload (" << frame.payloadLen << ") bytes: ";
        printBytes(frame.payload, frame.payloadLen, ostream);
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

    // Serialize buffer with ping frame, terminate the program on failure.
    {
        constexpr std::uint8_t pingDst{0x02U};
        constexpr std::uint8_t pingSrc{0x01U};
        constexpr std::uint8_t pingSeq{0x20U};
        if (!createPing(txBuf, txLen, pingDst, pingSrc, pingSeq)) { return -1;}
    }
    // Transmit ping, expect a pong as reponse, terminate the program on failure.
    {
        // Create RX buffer.
        std::uint8_t rxBuf[bufLen]{};
        std::size_t rxLen{sizeof(rxBuf)};

        // Transmit ping, expect a pong as reponse, terminate the program on failure.
        if (!transmitPing(txBuf, txLen, rxBuf, rxLen)) { return -1; }

        // Deserialize the response frame, terminate the program on failure.
        if (!validatePong(rxBuf, rxLen)) { return -1; }
    }
    return 0;
}
