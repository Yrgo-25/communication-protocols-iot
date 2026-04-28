/**
 * @brief Frame test program.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "comm/frame/def.h"
#include "comm/frame/frame.h"
#include "comm/frame/parser.h"

/** Buffer length in bytes. */
#define BUF_LEN 20U

/** Ping destination address. */
#define PING_DST 0x02U

/** Ping source address. */
#define PING_SRC 0x01U

/** Ping sequence number. */
#define PING_SEQ 0x20U

/** Simulated sensor value. */
#define SENSOR_VALUE (uint16_t)(100U)

/** Sensor value offset in payload. */
#define SENSOR_OFFSET 0U

/** Byte shift value for bit operations. */
#define BYTE_SHIFT 8U

// -----------------------------------------------------------------------------
static inline void write_u16(uint8_t* buf, const size_t offset, const uint16_t value)
{
    buf[offset]      = (uint8_t)(value >> BYTE_SHIFT);
    buf[offset + 1U] = (uint8_t)(value);
}

// -----------------------------------------------------------------------------
static inline uint16_t read_u16(const uint8_t* buf, const size_t offset)
{
    return ((uint16_t)(buf[offset]) << BYTE_SHIFT) | (uint16_t)(buf[offset + 1U]);
}

// -----------------------------------------------------------------------------
static void print_bytes(const uint8_t* bytes, const size_t byte_count)
{
    if ((NULL == bytes) || (0U == byte_count)) { return; }
    const size_t last = byte_count - 1U;
    printf("[");
    for (size_t i = 0U; i < last; ++i) { printf("%02X, ", bytes[i]); }
    printf("%02X]\n", bytes[last]);
}

// -----------------------------------------------------------------------------
static bool handle_frame(frame_t* tx_frame, frame_t* rx_frame)
{
    // Check the frames, return false if invalid.
    if ((NULL == tx_frame) || (NULL == rx_frame)) { return false; }

    // Check the TX frame type, respond accordingly.
    switch (tx_frame->type)
    {
        // If PING, respond with PONG.
        case FRAME_TYPE_PING:
        {
            rx_frame->type = FRAME_TYPE_PONG;
            rx_frame->len  = 0U;
            rx_frame->dst  = tx_frame->src;
            rx_frame->src  = tx_frame->dst;
            rx_frame->seq  = tx_frame->seq;
            break;
        }
        // If PONG, print the response in the terminal.
        case FRAME_TYPE_PONG:
        {
            printf("Node at address %u received PONG from node at address %u!\n", 
                tx_frame->dst, tx_frame->src);
            break;
        }
        // If status request, response with sensor value 100.
        case FRAME_TYPE_STATUS_REQUEST:
        {
            rx_frame->type = FRAME_TYPE_STATUS_RESPONSE;
            rx_frame->len  = sizeof(SENSOR_VALUE);
            rx_frame->dst  = tx_frame->src;
            rx_frame->src  = tx_frame->dst;
            rx_frame->seq  = tx_frame->seq;
            write_u16(rx_frame->data, SENSOR_OFFSET, SENSOR_VALUE);
            break;
        }
         // If status response, print the received sensor value.
        case FRAME_TYPE_STATUS_RESPONSE:
        {
            const uint16_t sensor_value = read_u16(tx_frame->data, SENSOR_OFFSET);
            printf("Node at address %u received sensor value %u from node at address %u!\n", 
                tx_frame->dst, sensor_value, tx_frame->src);
            break;
        }
        default:
        {
            printf("Unknown frame type!\n");
            return false;
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
static bool create_ping(uint8_t* buf, size_t* len, const uint8_t dst, 
                       const uint8_t src, const uint16_t seq)
{
    // Initialize ping frame structure.
    frame_t frame;
    frame_init(&frame);
    frame.type = FRAME_TYPE_PING;
    frame.dst  = dst;
    frame.src  = src;
    frame.seq  = seq;

    // Serialize buffer, return false on failure.
    const size_t frame_len = frame_serialize(&frame, buf, *len);
    if (0U == frame_len) { return false; }

    // Print the frame as bytes.
    printf("Ping frame (%zu bytes): ", frame_len);
    print_bytes(buf, frame_len);

    // Store the number of serialized bytes.
    *len = frame_len;
    return true;
}

// -----------------------------------------------------------------------------
bool transmit_ping(const uint8_t* txbuf, const size_t txlen, uint8_t* rxbuf, size_t* rxlen)
{
    // Deserialize TX data, expect a ping frame.
    frame_t tx_frame;
    frame_init(&tx_frame);
    if (!frame_deserialize(&tx_frame, txbuf, txlen)) { return false; }
    if (FRAME_TYPE_PING != tx_frame.type) { return false; }

    // Respond with a pong frame.
    frame_t rx_frame;
    frame_init(&rx_frame);
    rx_frame.type = FRAME_TYPE_PONG;
    rx_frame.dst  = tx_frame.src;
    rx_frame.src  = tx_frame.dst;
    rx_frame.seq  = tx_frame.seq;

    // Serialize RX data, return false on failure.
    *rxlen = frame_serialize(&rx_frame, rxbuf, *rxlen);
    if ((0U == *rxlen) || (FRAME_TYPE_PONG != rx_frame.type)) { return false; }

    // Store the number of serialized RX bytes.
    return true;
}

// -----------------------------------------------------------------------------
const char* type_str(const frame_type_t type)
{
    switch (type)
    {
        case FRAME_TYPE_PING:
            return "Ping";
        case FRAME_TYPE_PONG:
            return "Pong";
        case FRAME_TYPE_STATUS_REQUEST:
            return "StatusRequest";
        case FRAME_TYPE_STATUS_RESPONSE:
            return "StatusResponse";
        default:
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
bool validate_pong(const uint8_t* buf, const size_t len)
{
    // Deserialize response data, return false on failure.
    frame_t frame;
    frame_init(&frame);
    if (!frame_deserialize(&frame, buf, len)) { return false; }
    if (FRAME_TYPE_PONG != frame.type) { return false; }

    // Print the frame as bytes.
    printf("Pong frame (%zu bytes): ", len);
    print_bytes(buf, len);

    // Print deserialized data.
    printf("Deserialized frame:\n");
    printf("\t- Frame type: %s\n", type_str(frame.type));
    printf("\t- Destination address: %u\n", frame.dst);
    printf("\t- Source address: %u\n", frame.src);
    printf("\t- Sequence: %u\n", frame.seq);
    
    // Print payload (if any).
    if (0U < frame.len)
    {
        printf("\t- Payload (%u) bytes: ", frame.len);
        print_bytes(frame.data, frame.len);
    }
    return true;
}

/**
 * @brief Transmit ping and check response.
 * 
 * @return 0 on success, or -1 on failure.
 */
int main()
{
    // Create TX buffer.
    uint8_t txbuf[BUF_LEN];
    size_t txlen = sizeof(txbuf);

    // Serialize buffer with ping frame.
    frame_t tx_frame;
    frame_init(&tx_frame);
    if (!create_ping(txbuf, &txlen, PING_DST, PING_SRC, PING_SEQ)) { return -1; }

    // Parse frame byte by byte, extract parsed frame.
    {
        // Create frame parser.
        frame_parser_t parser;
        frame_parser_init(&parser);
        uint8_t byte_idx = 0U;

        // Parse frame byte by byte.
        while (!frame_parser_is_ready(&parser) && (BUF_LEN > byte_idx))
        {
            frame_parser_process_byte(&parser, txbuf[byte_idx++]);
        }
        // Extract parsed frame, return -1 on failure.
        if (!frame_parser_extract_frame(&parser, &tx_frame)) { return -1; }
    }

    // Transmit frame, expect pong as response.
    {
        // Handle parsed frame, return -1 on failure.
        frame_t rx_frame;
        frame_init(&rx_frame);
        if (!handle_frame(&tx_frame, &rx_frame)) { return -1; }
 
        // Serialize RX data.
        uint8_t rxbuf[BUF_LEN];
        size_t rxlen = sizeof(rxbuf);
        rxlen       = frame_serialize(&rx_frame, rxbuf, rxlen);

        // Deserialize the response frame, terminate the program on failure.
        if (!validate_pong(rxbuf, rxlen)) { return -1; }
    }
    return 0;
}
