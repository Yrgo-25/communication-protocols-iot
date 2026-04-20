/**
 * @brief Node stub implementation details.
 */
#include <cstdint>
#include <cstdio>

#include "comm/bus/interface.h"
#include "comm/frame/frame.h"
#include "comm/frame/parser.h"
#include "comm/node/stub.h"

namespace comm::node
{
// -----------------------------------------------------------------------------
bool Stub::handleFrame(const frame::Frame& txFrame, frame::Frame& rxFrame) const noexcept
{
    bool sendResponse{false};

    switch (txFrame.type)
    {
        case frame::Type::Ping:
        {
            constexpr std::size_t payloadLen{0U};
            rxFrame.type       = frame::Type::Pong;
            rxFrame.payloadLen = payloadLen;
            rxFrame.seqNr      = txFrame.seqNr;
            rxFrame.dstAddr    = txFrame.srcAddr;
            rxFrame.srcAddr    = txFrame.dstAddr;
            sendResponse       = true;
            std::printf("Node at address %u received PING from node at address %u!\n", 
                myAddr, txFrame.srcAddr);
            break;
        }
        case frame::Type::Pong:
        {
            std::printf("Node at address %u received PONG from node at address %u!\n", 
                myAddr, txFrame.srcAddr);
            break;
        }
        case frame::Type::StatusRequest:
        {
            constexpr std::size_t payloadLen{sizeof(myVal)};
            rxFrame.type        = comm::frame::Type::StatusResponse;
            rxFrame.payloadLen  = payloadLen;
            rxFrame.seqNr       = txFrame.seqNr;
            rxFrame.dstAddr     = txFrame.srcAddr;
            rxFrame.srcAddr     = txFrame.dstAddr;
            rxFrame.payload[0U] = highByte(myVal);
            rxFrame.payload[1U] = lowByte(myVal);
            sendResponse        = true;
            std::printf("Node at address %u received status request from node at address %u!\n",
                myAddr, txFrame.srcAddr); 
            break;
        }
        case frame::Type::StatusResponse:
        {
            const std::uint16_t sensorVal{readU16(txFrame.payload)};
            std::printf("Node at address %u received status response %u from node at address %u!\n", 
                myAddr, sensorVal, txFrame.srcAddr);
            break;
        }
        default:
            std::printf("Unknown frame type with ID %u!\n", 
                static_cast<std::uint8_t>(txFrame.type));
           break;
    }
    return sendResponse;
}
} // namespace comm::node
