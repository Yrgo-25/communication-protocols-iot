#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "comm/frame/def.h"
#include "comm/frame/frame.h"

/** Constant to shift bytes. */
#define BYTE_SHIFT 8U

/**
 * @brief Frame field structure.
 */
typedef struct
{
    /** Start-Of-Frame. */
    uint16_t sof;

    /** Sequence number. */
    uint16_t seq;

    /** Frame type (numeric value). */
    uint8_t type;

    /** Frame length. */
    uint8_t len;

    /** Destination address. */
    uint8_t dst;

    /** Source address. */
    uint8_t src;

    /** Checksum. */
    uint16_t chk;
} frame_fields_t;

// -----------------------------------------------------------------------------
static inline void write_u8(uint8_t* buf, const uint8_t offset, const uint8_t value)
{
    buf[offset] = value;
}

// -----------------------------------------------------------------------------
static inline void write_u16(uint8_t* buf, const uint8_t offset, const uint16_t value)
{
    buf[offset]      = (uint8_t)(value >> BYTE_SHIFT);
    buf[offset + 1U] = (uint8_t)(value);
}

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
static void init_data(frame_t* self)
{
    for (uint8_t i = 0U; i < FRAME_MAX_DATA_LEN; ++i) { self->data[i] = 0U; }
}

// -----------------------------------------------------------------------------
static void write_data(uint8_t* buf, const uint8_t* data, const uint8_t len)
{
    for (uint8_t i = 0U; i < len; ++i) { buf[FRAME_OFFSET_DATA + i] = data[i]; }
}

// -----------------------------------------------------------------------------
static void read_data(uint8_t* data, const uint8_t* buf, const uint8_t len)
{
    for (uint8_t i = 0U; i < len; ++i) { data[i] = buf[FRAME_OFFSET_DATA + i]; }
}

// -----------------------------------------------------------------------------
static uint16_t compute_chk(const uint8_t* buf, const uint8_t chk_offset)
{
    uint16_t chk = 0U;
    for (uint8_t i = 0U; i < chk_offset; ++i) { chk += buf[i]; }
    return chk;
}

// -----------------------------------------------------------------------------
void frame_init(frame_t* self)
{
    if (NULL == self) { return; }
    self->len  = 0U;
    self->type = FRAME_TYPE_UNKNOWN;
    self->dst  = 0U;
    self->src  = 0U;
    self->seq  = 0U;
    init_data(self);
}

// -----------------------------------------------------------------------------
size_t frame_serialize(const frame_t* self, uint8_t* buf, size_t len)
{
    if ((NULL == self) || (NULL == buf) || (0U == len)) { return 0U; }
    if ((FRAME_MIN_LEN + self->len) > len) { return 0U; }

    write_u16(buf, FRAME_OFFSET_SOF, FRAME_SOF);
    write_u8(buf, FRAME_OFFSET_LEN, self->len);
    write_u8(buf, FRAME_OFFSET_TYPE, (uint8_t)(self->type));
    write_u8(buf, FRAME_OFFSET_DST, self->dst);
    write_u8(buf, FRAME_OFFSET_SRC, self->src);
    write_u16(buf, FRAME_OFFSET_SEQ, self->seq);
    write_data(buf, self->data, self->len);
    const uint8_t chk_offset = FRAME_OFFSET_CHK(self->len);
    const uint16_t chk       = compute_chk(buf, chk_offset);
    write_u16(buf, chk_offset, chk);
    return chk_offset + sizeof(chk);
}

// -----------------------------------------------------------------------------
bool frame_deserialize(frame_t* self, const uint8_t* buf, const size_t len)
{
    if ((NULL == self) || (NULL == buf)) { return false; }
    if ((FRAME_MIN_LEN) > len) { return false; }

    // Create frame header structure for parsing frame date from the buffer.
    frame_fields_t fields;

    // Parse SOF, return false if invalid.
    fields.sof = read_u16(buf, FRAME_OFFSET_SOF);
    if (FRAME_SOF != fields.sof) { return false; }

    // Parse payload length, return false if invalid.
    fields.len = read_u8(buf, FRAME_OFFSET_LEN);
    if (FRAME_MAX_DATA_LEN < fields.len) { return false; }

    // Compute frame length, return false if the buffer is to small to hold the entire frame.
    const size_t frame_len = FRAME_MIN_LEN + fields.len;
    if (len < frame_len) { return false; }

    // Parse frame type, return false if invalid.
    fields.type = read_u8(buf, FRAME_OFFSET_TYPE);
    if (FRAME_TYPE_UNKNOWN <= fields.type) { return false; }

    // Read destination and source address.
    fields.dst = read_u8(buf, FRAME_OFFSET_DST);
    fields.src = read_u8(buf, FRAME_OFFSET_SRC);
    fields.seq = read_u16(buf, FRAME_OFFSET_SEQ);

    // Compute checksum.
    const size_t chk_offset = FRAME_OFFSET_CHK(fields.len);
    const uint16_t chk      = compute_chk(buf, chk_offset);

    // Parse checksum, return false on mismatch.
    fields.chk = read_u16(buf, chk_offset);
    if (chk != fields.chk) { return false; }

    // Copy header.
    self->len  = fields.len;
    self->type = (frame_type_t)(fields.type);
    self->dst  = fields.dst;
    self->src  = fields.src;
    self->seq  = fields.seq;

    // Copy payload, then return true to indicate success.
    read_data(self->data, buf, self->len);
    return true;
}
