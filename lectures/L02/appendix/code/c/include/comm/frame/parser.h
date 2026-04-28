/**
 * @brief Frame parser implementation.
 */
#ifndef COMM_FRAME_PARSER_H_
#define COMM_FRAME_PARSER_H_

#include <stdbool.h>
#include <stdint.h>

#include "comm/frame/def.h"
#include "comm/frame/frame.h"

/**
 * @brief Frame parser structure.
 */
typedef struct frame_parser
{
    /** Buffer holding parsed frame data. */
    uint8_t buf[FRAME_MAX_LEN];

    /** Number of parsed bytes. */
    uint8_t parsed_bytes;

    /** Parser state ID. */
    uint8_t state_id;
} frame_parser_t;

/**
 * @brief Initialize frame parser.
 * 
 * @param[out] self Pointer to the parser.
 */
void frame_parser_init(frame_parser_t* self);

/**
 * @brief Check if a complete frame has been parsed.
 *
 * @param[in] self Pointer to the parser.
 * 
 * @return True if a complete frame has been parsed, false otherwise.
 */
bool frame_parser_is_ready(const frame_parser_t* self);

/**
 * @brief Process next byte of the frame being parsed.
 *
 * @param[out] self Pointer to the parser.
 * @param[in] byte Next byte to parse.
 *
 * @return True on success, false on failure.
 */
bool frame_parser_process_byte(frame_parser_t* self, uint8_t byte);

/**
 * @brief Extract parsed frame.data.
 *
 * @param[in] self Pointer to the parser.
 * @param[out] frame Pointer to parsed frame data.
 *
 * @return True on success, false on failure.
 */
bool frame_parser_extract_frame(const frame_parser_t* self, frame_t* frame);

/**
 * @brief Reset the parser.
 * 
 * @param[in] self Pointer to the parser.
 */
void frame_parser_reset(frame_parser_t* self);

#endif /** COMM_FRAME_PARSER_H_ */
