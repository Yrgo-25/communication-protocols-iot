/**
 * @brief Data bus demo.
 */
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "comm/bus/stub.h"
#include "comm/frame/frame.h"
#include "comm/frame/type.h"
#include "comm/node/stub.h"

using namespace comm;

/**
 * @brief Transmit status request via bus and check response.
 * 
 * @return 0 on success, or -1 on failure.
 */
int main()
{
    constexpr std::uint8_t node1Addr{1U};
    constexpr std::uint8_t node2Addr{2U};
    constexpr std::uint8_t node3Addr{3U};

    constexpr std::uint16_t node1Val{0x100U};
    constexpr std::uint16_t node2Val{0x200U};
    constexpr std::uint16_t node3Val{0x300U};

    // Create bus and three nodes.
    bus::Stub bus{};
    node::Stub node1{node1Addr, bus, node1Val};
    node::Stub node2{node2Addr, bus, node2Val};
    node::Stub node3{node3Addr, bus, node3Val};

    // Send status request from node 1 to node 2.
    {
        // Create status request frame.
        constexpr std::uint16_t seqNr{0x20U};
        frame::Frame txFrame{};
        txFrame.type    = frame::Type::StatusRequest;
        txFrame.dstAddr = node2Addr;
        txFrame.srcAddr = node1Addr;
        txFrame.seqNr   = seqNr;
        
        // Serialize frame into transmit buffer; abort if serialization fails.
        constexpr std::size_t bufLen{20U};
        std::uint8_t txBuf[bufLen]{};
        const std::size_t txLen{txFrame.serialize(txBuf, bufLen)};
        if (0U == txLen) { return -1; }

        // Send serialized frame over the bus one byte at a time.
        for (std::size_t i{}; i < txLen; ++i) { bus.sendByte(txBuf[i]); }
    }

    // Run 50 clockcycles, expect a status response from node 2.
    constexpr std::size_t clockCycleCount{50U};
    for (std::size_t i{}; i < clockCycleCount; ++i)
    {
        node1.tick();
        node2.tick();
        node3.tick();
        bus.tick();
    }
    return 0;
}
