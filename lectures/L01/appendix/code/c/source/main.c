/**
 * @brief Frame test program.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "comm/frame/def.h"
#include "comm/frame/frame.h"

/** Buffer length in bytes. */
#define BUF_LEN 20U

/** Ping destination address. */
#define PING_DST 0x02U

/** Ping source address. */
#define PING_SRC 0x01U

/** Ping sequence number. */
#define PING_SEQ 0x20U

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

    // Serialize buffer with ping frame, terminate the program on failure.
    {
        if (!create_ping(txbuf, &txlen, PING_DST, PING_SRC, PING_SEQ)) { return -1;}
    }
    // Transmit ping, expect a pong as reponse, terminate the program on failure.
    {
        // Create RX buffer.
        uint8_t rxbuf[BUF_LEN];
        size_t rxlen = sizeof(rxbuf);

        // Transmit ping, expect a pong as reponse, terminate the program on failure.
        if (!transmit_ping(txbuf, txlen, rxbuf, &rxlen)) { return -1; }

        // Deserialize the response frame, terminate the program on failure.
        if (!validate_pong(rxbuf, rxlen)) { return -1; }
    }
    return 0;
}
