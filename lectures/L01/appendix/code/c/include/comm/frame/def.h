/**
 * @brief Frame definitions.
 */
#ifndef COMM_FRAME_DEF_H_
#define COMM_FRAME_DEF_H_

/** Frame header length. */
#define FRAME_HEADER_LEN 8U

/** Frame footer length. */
#define FRAME_FOOTER_LEN   2U

/** Maximum data field (payload) length. */
#define FRAME_MAX_DATA_LEN 32U

/** Minimum frame length (no payload). */
#define FRAME_MIN_LEN (FRAME_HEADER_LEN + FRAME_FOOTER_LEN)

/** Maximum frame length (max payload). */
#define FRAME_MAX_LEN (FRAME_MIN_LEN + FRAME_MAX_DATA_LEN)

/** Start-Of-Frame value. */
#define FRAME_SOF 0xA5F7U

/** Offset for the SOF field. */
#define FRAME_OFFSET_SOF 0U

/** Offset for the LEN field. */
#define FRAME_OFFSET_LEN 2U

/** Offset for the TYPE field. */
#define FRAME_OFFSET_TYPE 3U

/** Offset for the DST field. */
#define FRAME_OFFSET_DST 4U

/** Offset for the SRC field. */
#define FRAME_OFFSET_SRC 5U

/** Offset for the SEQ field. */
#define FRAME_OFFSET_SEQ 6U

/** Offset for the DATA field. */
#define FRAME_OFFSET_DATA 8U

/** 
 * @brief Get offset of the CHK field based on the length of the DATA field.
 * 
 * @param[in] data_len Length of the DATA field.
 * 
 * @return Offset of the CHK field.
 */
#define FRAME_OFFSET_CHK(data_len) (FRAME_OFFSET_DATA + (data_len))

/**
 * @brief Enumeration of frame types.
 */
typedef enum frame_type
{
    FRAME_TYPE_PING,            ///< Ping frame.
    FRAME_TYPE_PONG,            ///< Pong frame.
    FRAME_TYPE_STATUS_REQUEST,  ///< Status request frame.
    FRAME_TYPE_STATUS_RESPONSE, ///< Status response frame.
    FRAME_TYPE_UNKNOWN,         ///< Unknown frame.
} frame_type_t;

#endif /** COMM_FRAME_DEF_H_ */
