/**
 * @brief Frame parser implementation details.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "comm/frame/def.h"
#include "comm/frame/frame.h"
#include "comm/frame/parser.h"

/** Constant to shift bytes. */
#define BYTE_SHIFT 8U

/**
 * @brief Enumeration of parser states.
 */
typedef enum state
{
    STATE_WAIT_FOR_SOF1,    ///< Wait for Start-of-Frame (high byte).
    STATE_WAIT_FOR_SOF2,    ///< Wait for Start-of-Frame (low byte).
    STATE_WAIT_FOR_LEN,     ///< Wait for payload length.
    STATE_WAIT_FOR_TYPE,    ///< Wait for frame type.
    STATE_WAIT_FOR_DST,     ///< Wait for destination address.
    STATE_WAIT_FOR_SRC,     ///< Wait for source address.
    STATE_WAIT_FOR_SEQ1,    ///< Wait for sequence number (high byte).
    STATE_WAIT_FOR_SEQ2,    ///< Wait for sequence number (low byte).
    STATE_WAIT_FOR_DATA,    ///< Wait for payload (len bytes).
    STATE_WAIT_FOR_CHK1,    ///< Wait for checksum (high byte).
    STATE_WAIT_FOR_CHK2,    ///< Wait for checksum (low byte).
    STATE_READY,            ///< Frame ready.
} state_t;

// -----------------------------------------------------------------------------
static inline uint8_t state_id(const state_t state) { return (uint8_t)(state); }

// -----------------------------------------------------------------------------
static inline uint8_t read_u8(const uint8_t* buf, const uint8_t offset)
{
    return buf[offset];
}


// -----------------------------------------------------------------------------
static inline uint16_t read_u16(const uint8_t* buf, const uint8_t offset)
{
    return ((uint16_t)(buf[offset]) << BYTE_SHIFT) | (uint16_t)(buf[offset + 1U]);
}

// -----------------------------------------------------------------------------
static inline uint8_t get_msb(const uint16_t value)
{
    return (uint8_t)(value >> BYTE_SHIFT);
}

// -----------------------------------------------------------------------------
static inline uint8_t get_lsb(const uint16_t value)
{
    return (uint8_t)(value);
}

// -----------------------------------------------------------------------------
static void init_array(uint8_t* arr, size_t len)
{
    for (size_t i = 0U; i < len; ++i) { arr[i] = 0U; }
}

// -----------------------------------------------------------------------------
static void parse_data(frame_t* frame, const uint8_t* buf, const uint8_t len)
{
    for (uint8_t i = 0U; i < len; ++i)
    {
        frame->data[i] = buf[FRAME_OFFSET_DATA + i];
    }
}

// -----------------------------------------------------------------------------
static bool handle_parse_error(frame_parser_t* self)
{
    frame_parser_reset(self);
    return false;
}

// -----------------------------------------------------------------------------
void frame_parser_init(frame_parser_t* self) { frame_parser_reset(self); }

// -----------------------------------------------------------------------------
bool frame_parser_is_ready(const frame_parser_t* self)
{
    // Return true if the current state is set to READY.
    return NULL != self ? state_id(STATE_READY) == self->state_id : false;
}

// -----------------------------------------------------------------------------
bool frame_parser_process_byte(frame_parser_t* self, const uint8_t byte)
{
    // Check the frame parser, return false if invalid.
    if (NULL == self) { return false; }

    // Read the current state.
    const state_t state = (state_t)(self->state_id);

    // Parse the given byte basen on current state.
    switch (state)
    {
        case STATE_WAIT_FOR_SOF1:
        {
            // Check if given byte matches first byte of SOF1, handle parse error if not.
            const uint8_t sof1 = get_msb(FRAME_SOF);
            if (sof1 != byte) { return handle_parse_error(self); }
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_WAIT_FOR_SOF2:
        {
            // Check if given byte matches the second byte of SOF1, handle parse error if not.
            const uint8_t sof2 = get_lsb(FRAME_SOF);
            if (sof2 != byte) { return handle_parse_error(self); }
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_WAIT_FOR_LEN:
        {
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_WAIT_FOR_TYPE:
        {
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_WAIT_FOR_DST:
        {
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_WAIT_FOR_SRC:
        {
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_WAIT_FOR_SEQ1:
        {
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_WAIT_FOR_SEQ2:
        {
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_WAIT_FOR_DATA:
        {
            // Go immediately to next state if the entire payload has been parsed.
            const uint8_t len        = self->buf[FRAME_OFFSET_LEN];
            const uint8_t parsed_len = self->parsed_bytes - FRAME_OFFSET_DATA;
            if (parsed_len >= len) { self->state_id++; }

            // Parse byte as payload.
            // Remain in the current state until the entire payload has been parsed.
            else 
            { 
                self->buf[self->parsed_bytes++] = byte;
                break; 
            }
        }
        case STATE_WAIT_FOR_CHK1:
        {
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_WAIT_FOR_CHK2:
        {
            self->buf[self->parsed_bytes++] = byte;
            self->state_id++;
            break;
        }
        case STATE_READY:
        {
            // Frame already complete, skip processing byte.
            return false;
        }
        default:
        {
            return handle_parse_error(self);
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
bool frame_parser_extract_frame(const frame_parser_t* self, frame_t* frame)
{
    // Check if a frame is ready to be parsed, return false if not.
    if (!frame_parser_is_ready(self)) { return false; }

    // Check the frame to parse, return false if invalid.
    if (NULL == frame) { return false; }
    
    // Parse the frame header (excluding SOF).
    frame->len  = read_u8(self->buf, FRAME_OFFSET_LEN);
    frame->type = (frame_type_t)(read_u8(self->buf, FRAME_OFFSET_TYPE));
    frame->dst  = read_u8(self->buf, FRAME_OFFSET_DST);
    frame->src  = read_u8(self->buf, FRAME_OFFSET_SRC);
    frame->dst  = read_u8(self->buf, FRAME_OFFSET_DST);
    frame->seq  = read_u16(self->buf, FRAME_OFFSET_SEQ);

    // Parse the payload, then return true to indicate success.
    parse_data(frame, self->buf, frame->len);
    return true;
}

// -----------------------------------------------------------------------------
void frame_parser_reset(frame_parser_t* self)
{
    // Skip reset if the frame is invalid.
    if (NULL == self) { return; }

    // Reset the member variables.
    init_array(self->buf, FRAME_MAX_LEN);
    self->parsed_bytes = 0U;
    self->state_id     = state_id(STATE_WAIT_FOR_SOF1);
}
