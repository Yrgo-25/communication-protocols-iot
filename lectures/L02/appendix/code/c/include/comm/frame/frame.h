/**
 * @brief Frame implementation.
 */
#ifndef COMM_FRAME_FRAME_H_
#define COMM_FRAME_FRAME_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "comm/frame/def.h"

/**
 * @brief Frame structure.
 */
typedef struct frame
{
    /** Frame data (payload). */
    uint8_t data[FRAME_MAX_DATA_LEN];

    /** Frame type. */
    frame_type_t type;

    /** Sequence number. */
    uint16_t seq;

    /** Frame length. */
    uint8_t len;

    /** Destination address. */
    uint8_t dst;

    /** Source address. */
    uint8_t src;
} frame_t;

/**
 * @brief Initialize empty frame.
 * 
 * @param[out] self Frame to initialize. 
 */
void frame_init(frame_t* self);

/**
 * @brief Serialize frame.
 * 
 * @param[in] self Frame to serialize.
 * @param[out] buf Buffer holding serialized frame data.
 * @param[in] len Buffer length. Must be large enough to contain the complete frame.
 * 
 * @return The frame length, or 0 on failure.
 */
size_t frame_serialize(const frame_t* self, uint8_t* buf, size_t len);

/**
 * @brief Deserialize frame.
 * 
 * @param[out] self Frame to deserialize.
 * @param[in] buf Buffer holding frame data to deserialize.
 * @param[in] len Buffer length. Must be large enough to contain the complete frame.
 * 
 * @return True on success, false on failure.
 */
bool frame_deserialize(frame_t* self, const uint8_t* buf, size_t len);

#endif /** COMM_FRAME_FRAME_H_ */
