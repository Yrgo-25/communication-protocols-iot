/**
 * @brief Frame parser implementation.
 */
#pragma once

#include <cstdint>

#include "comm/frame/def.h"

namespace comm::frame
{
/** Frame structure. */
struct Frame;

/**
 * @brief Frame parser implementation.
 * 
 *        This class is non-copyable and non-movable.
 */
class Parser final
{
public:
    /**
     * @brief Constructor.
     */
    Parser() noexcept;

    /**
     * @brief Destructor.
     */
    ~Parser() noexcept = default;

    /**
     * @brief Check if a complete frame has been parsed.
     *
     * @return True if a complete frame has been parsed, false otherwise.
     */
    bool isFrameReady() const noexcept;

    /**
     * @brief Process next byte of the frame being parsed.
     *
     * @param[in] byte Next byte to parse.
     *
     * @return True on success, false on failure.
     */
    bool processByte(std::uint8_t byte) noexcept;

    /**
     * @brief Extract parsed frame.data.
     *
     * @param[out] frame Parsed frame data.
     *
     * @return True on success, false on failure.
     */
    bool extractFrame(Frame& frame) const noexcept;

    /**
     * @brief Reset the parser.
     */
    void reset() noexcept;

    Parser(const Parser&)            = delete; // No copy constructor.
    Parser(Parser&&)                 = delete; // No move constructor.
    Parser& operator=(const Parser&) = delete; // No copy assignment.
    Parser& operator=(Parser&&)      = delete; // No move assignment.

private:
    /**
     * @brief Enumeration of parser states.
     */
    enum class State : std::uint8_t
    {
        WaitForSof1,    ///< Wait for Start-of-Frame (high byte).
        WaitForSof2,    ///< Wait for Start-of-Frame (low byte).
        WaitForLen,     ///< Wait for payload length.
        WaitForType,    ///< Wait for frame type.
        WaitForDst,     ///< Wait for destination address.
        WaitForSrc,     ///< Wait for source address.
        WaitForSeq1,    ///< Wait for sequence number (high byte).
        WaitForSeq2,    ///< Wait for sequence number (low byte).
        WaitForPayload, ///< Wait for payload (len bytes).
        WaitForChk1,    ///< Wait for checksum (high byte).
        WaitForChk2,    ///< Wait for checksum (low byte).
        Ready,          ///< Frame ready.
    };

    /** Buffer holding parsed frame data. */
    std::uint8_t myBuf[Frame::MaxFrameLen];

    /** Number of parsed bytes. */
    std::uint8_t myParsedBytes;

    /** Parser state. */
    State myState;
};
} // namespace comm::frame
