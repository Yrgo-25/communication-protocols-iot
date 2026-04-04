# Bilaga A

## Frames i inbyggda system
I inbyggda system kommunicerar noder ofta via seriella gränssnitt såsom UART, SPI, I2C eller RS‑485. Data skickas då som en sekvens av bytes. För att kunna tolka denna byte‑ström behövs en struktur. En sådan struktur kallas **frame**.

En frame är en strukturerad representation av data som:
* Innehåller tydliga fält.
* Har en definierad längd.
* Kan valideras (t.ex. via en checksumma).
* Kan serialiseras till en byte‑array (`std::uint8_t[]`).
* Kan deserialiseras från en byte‑array.

---

### Lagerindelning

Nedanstående modell visar de olika lagren som förekommer när data skickas:

```
Lager         Exempel                Vad gör det?
------------- ---------------------- -----------------------------
Physical      UART, RS‑485           Skickar elektriska signaler
Transport     Byte‑ström             Flyttar bytes (utan struktur)
Protocol      Frame + Parser         Tolkar struktur i en byte‑ström
Application   PING, STATUS_REQUEST   Användarlogik
```

**OBS:** Med transport menas här byteförflyttning över ett fysiskt interface (t.ex. UART/RS-485), inte transportlagret i OSI-modellen (TCP/UDP).

Frames och parser ligger i protokoll-lagret.

---

### Vad är en frame?
En frame är ett strukturerat datapaket med fält som gör att mottagaren kan:
1) Hitta början (SOF).
2) Veta hur lång framen är (LEN).
3) Förstå vad den betyder (TYPE + headerfält).
4) Validera att datan inte är trasig (CHK).

I kursen använder vi följande framestruktur:

| Fält | Storlek | Beskrivning |
|---|---:|---|
| SOF | 2 byte | Start-of-frame, värde `0xA5F7` |
| LEN | 1 byte | Payload-längd i bytes |
| TYPE | 1 byte | Meddelandetyp |
| DST | 1 byte | Destination address |
| SRC | 1 byte | Source address |
| SEQ | 2 byte | Sekvensnummer (matchar request/response) |
| DATA | N byte | Payload |
| CHK | 2 byte | Checksumma (summering av bytes) |


Gällande fältet:
* Fält större än 1 byte (SOF, SEQ samt CHK) skickas big-endian, dvs. den mest signifikanta byten (MSB) skickas först i byteföljden.
* LEN räknar endast DATA, dvs. payload-längden, inte header eller checksum.
* CHK beräknas över allt utom checksumfältet:
    * `CHK = SUM(SOF + LEN + TYPE + DST + SRC + SEQ + DATA)`.
    * Checksumman beräknas över den serialiserade byteföljden, byte för byte — inte över fältvärden som heltal. Som exempel, SOF-beräknas `0xA5 + 0xF7`, inte `0XA5F7` som ett heltal.
    * Summeringen sker modulo enligt `2^16`, dvs. overflow wrappar.

Sammanfattat:

```text
Fält   Storlek   Beskrivning
------ --------- --------------------------
SOF    2 byte    Start-of-Frame (synkord)
LEN    1 byte    Payload-längd
TYPE   1 byte    Meddelandetyp
DST    1 byte    Destination address
SRC    1 byte    Source address
SEQ    2 byte    Sekvensnummer
DATA   N byte    Payload
CHK    2 byte    Checksumma
```

Varför behövs **DST/SRC/SEQ**?
* **DST/SRC**: Behövs för att routa och identifiera noder på en delad buss.
* **SEQ**: Behövs för att koppla ihop svar med rätt request (och vid behov retry/ACK, som behandlas senare i kursen).

---

### Val av SOF
För att minska risken för falsk synk används ofta två bytes som SOF:
* I detta exempel används `0xA5F7`.
* Det första som kontrolleras när en frame läses är SOF; om detta inte matchar förkastas datan och parsern fortsätter leta efter nästa SOF.

---

### Exempel: PING utan payload

Antag:
* `SOF = 0xA5F7`
* `LEN = 0x00` (ingen payload)
* `TYPE = 0x00` (PING)
* `DST = 0x02` (skickar PING till enhet B)
* `SRC = 0x01` (skickar PING från enhet A)
* `SEQ = 0x20`
* `CHK = 0xA5 + 0xF7 + 0x00 + 0x00 + 0x02 + 0x01 + 0x00 + 0x20 = 0x01bf`

Då ser framen ut (CHK beror på summeringen):

```
SOF      LEN   TYPE   DST   SRC   SEQ      DATA      CHK
A5 F7    00    00     02    01    00 20    --        01 bf
```

Och ett PONG-svar blir typiskt:
* `SOF = 0xA5F7`
* `LEN = 0x00` (ingen payload)
* `TYPE = 0x01` (PONG)
* `DST = 0x01` (skickar PONG till enhet A)
* `SRC = 0x02` (skickar PONG från enhet B)
* `SEQ = 0x0020`
* `CHK = 0xA5 + 0xF7 + 0x00 + 0x01 + 0x02 + 0x01 + 0x00 + 0x20 = 0x01c0`

---

### Varför behövs frames?
Vid seriell kommunikation får vi en rå byte‑ström, t.ex.:

```
01 00 02 34 12 FF 9A ...
```

Utan struktur vet vi inte:
* Var meddelandet börjar.
* Hur långt det är.
* Vad det betyder.
* Om det mottogs korrekt.

En frame löser detta genom:
* `SOF` → Synk till början av en frame.
* `LEN` → Bestämmer hur många bytes som ska läsas.
* `TYPE` → Gör att vi kan dispatch:a/route:a.
* `CHK` → Upptäcker fel i överföringen.

---

### Implementering av frametyper
För att implementera frametyper kan vi med fördel använda enumerationsklasser.  
Nedan visas en sådan enumerationsklass `Type`, där respektive enumerator utgör ett 8-bitars heltal.
Vi låter det högsta värdet utgöras av `Type::Unknown`, så att vi enkelt kan verifiera att en given
typ är giltig:

```cpp
/**
 * @brief Enumeration of frame types.
 */
enum class Type : std::uint8_t
{
    Ping,           ///< Ping frame.
    Pong,           ///< Pong frame.
    StatusRequest,  ///< Status request frame.
    StatusResponse, ///< Status response frame.
                    ///< Add more frame types here.
    Unknown,        ///< Unknown frame.
};
```

För att kolla om en given typ `type` är giltig kan vi sedan kontrollera att denna är mindre än
det numeriska värdet av `Type::Unknown`:

```cpp
/**
 * @brief Check whether given type is valid.
 * 
 * @param[in] type The type to check.
 * 
 * @return True if the type is valid, false otherwise.
 */
constexpr bool isTypeValid(const Type type) noexcept
{
    return static_cast<std::uint8_t>(Type::Unknown) > static_cast<std::uint8_t>(type);
}
```

### En enkel checksumma‑algoritm (16‑bit)
För kurs/labb räcker ofta en checksumma som är:
* Enkel att implementera.
* Enkel att testa.
* Tillräcklig för att upptäcka många slumpmässiga bitfel.

Vi använder en **16‑bitars summering**:
* Startvärde = `0U`.
* För varje byte i området: `sum += byte`.
* Resultatet är checksumman (modulo `2^16`).

Detta är inte lika starkt som exempelvis CRC, men räcker för att träna framing, parsing och robust felhantering.

#### Checksumma-funktion

```cpp
#include <cstddef>
#include <cstdint>

/**
 * @brief Compute a simple 16-bit checksum (sum of bytes modulo 2^16).
 *
 * @param[in] data Pointer to bytes to checksum.
 * @param[in] dataLen Number of bytes to checksum.
 *
 * @return 16-bit checksum.
 */
std::uint16_t checksum16(const std::uint8_t* data, const std::size_t dataLen) noexcept
{
    if ((nullptr == data) || (0U == dataLen)) { return 0U; }
    std::uint16_t checksum{};
    for (std::size_t i{}; i < dataLen; ++i) { checksum += data[i]; }
    return checksum;
}
```

---

### Serialisering
Serialisering innebär att konvertera strukturerad data till en sekvens av bytes (`std::uint8_t[]`) så att den kan skickas över ett gränssnitt.

I inbyggda system används nästan alltid:
* C‑arrayer (`std::uint8_t buffer[Size]`),
* Pekare till arrayer vid funktionsanrop (`std::uint8_t*`),
* Explicit längd (`std::size_t`).

#### Serialiseringsfunktion

Nedan visas en serialiseringsfunktion `serializePing()`, som serialiserar en buffer med en
ping-frame:
* Angiven buffer fylls med:
    * SOF: `0xA5F7` i detta protokoll.
    * Payload-längd: `0`, då ping-frames saknar payload.
    * Frametyp: Ping, vilket motsvarar `0x00`, då `Type::Ping` utgör den första enumeratorn i enumerationsklassen `Type`.
    * Destinationadress: Sätts vid funktionsanropet, exempelvis `0x02`.
    * Avsändaradress: Sätts vid funktionsanropet, exempelvis `0x01`.
    * Sekvens: Sätts funktionsanropet, exempelvis `0x0020`.
* Funktionen returnerar antalet serialiserade bytes.

```cpp
/**
 * @brief Serialize a ping frame.
 * 
 * @param[out] buf Buffer to serialize.
 * @param[in] bufLen Length of the buffer.
 * @param[in] dstAddr Destination address.
 * @param[in] srcAddr Source address.
 * @param[in] seqNr Sequence number.
 * 
 * @return The number of serialized bytes.
 */
std::size_t serializePing(std::uint8_t* buf, const std::size_t bufLen,
                          const std::uint8_t dstAddr, const std::uint8_t srcAddr,
                          const std::uint16_t seqNr) noexcept
{
    // No payload in PING -> frame length equals the baseline length.
    constexpr std::size_t payloadLen{0U};
    constexpr std::size_t frameLen{BaselineLen + payloadLen};
    if ((nullptr == buf) || (frameLen > bufLen)) { return 0U; }

    // Add SOF.
    constexpr std::uint16_t sof{0xA5F7U};
    buf[0U] = static_cast<std::uint8_t>((sof >> 8U) & 0xFFU);
    buf[1U] = static_cast<std::uint8_t>(sof & 0xFFU);

    // Add payload length.
    buf[2U] = static_cast<std::uint8_t>(payloadLen);

    // Add frame type.
    buf[3U] = static_cast<std::uint8_t>(Type::Ping);

    // Add destination and source address.
    buf[4U] = dstAddr;
    buf[5U] = srcAddr;

    // Add sequence number.
    buf[6U] = static_cast<std::uint8_t>((seqNr >> 8U) & 0xFFU);
    buf[7U] = static_cast<std::uint8_t>(seqNr & 0xFFU);

    // No payload in ping frames.

    // Add checksum.
    constexpr std::size_t checksumOffset{8U};
    const std::uint16_t checksum{checksum16(buf, checksumOffset)};
    buf[8U] = static_cast<std::uint8_t>((checksum >> 8U) & 0xFFU);
    buf[9U] = static_cast<std::uint8_t>((checksum) & 0xFFU);

    // Return the number of serialized bytes.
    return frameLen;
}
```

#### Exempel: Serialisering av en PING-frame

```cpp
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace
{
namespace comm::frame
{
// Enumeration of frame types.
enum class Type : std::uint8_t
{
    Ping,
    Pong,
    StatusRequest,
    StatusResponse,
    Unknown,
};

// Start-of-Field value.
constexpr std::uint16_t SofVal{0xA5F7U};

// Header = SOF + LEN + TYPE + DST + SRC + SEQ.
constexpr std::size_t HeaderLen{8U};

// Footer = CHK.
constexpr std::size_t FooterLen{2U};

// Baseline = header + footer.
constexpr std::size_t BaselineLen{HeaderLen + FooterLen};

// -----------------------------------------------------------------------------
std::uint16_t checksum16(const std::uint8_t* data, const std::size_t dataLen) noexcept
{
    if ((nullptr == data) || (0U == dataLen)) { return 0U; }
    std::uint16_t checksum{};
    for (std::size_t i{}; i < dataLen; ++i) { checksum += data[i]; }
    return checksum;
}

// -----------------------------------------------------------------------------
std::size_t serializePing(std::uint8_t* buf, const std::size_t bufLen,
                          const std::uint8_t dstAddr, const std::uint8_t srcAddr,
                          const std::uint16_t seqNr) noexcept
{
    // No payload in PING -> frame length equals the baseline length.
    constexpr std::size_t payloadLen{0U};
    constexpr std::size_t frameLen{BaselineLen + payloadLen};
    if ((nullptr == buf) || (frameLen > bufLen)) { return 0U; }

    // Add SOF.
    buf[0U] = static_cast<std::uint8_t>((SofVal >> 8U) & 0xFFU);
    buf[1U] = static_cast<std::uint8_t>(SofVal & 0xFFU);

    // Add payload length.
    buf[2U] = static_cast<std::uint8_t>(payloadLen);

    // Add frame type.
    buf[3U] = static_cast<std::uint8_t>(Type::Ping);

    // Add destination and source address.
    buf[4U] = dstAddr;
    buf[5U] = srcAddr;

    // Add sequence number.
    buf[6U] = static_cast<std::uint8_t>((seqNr >> 8U) & 0xFFU);
    buf[7U] = static_cast<std::uint8_t>(seqNr & 0xFFU);

    // No payload in ping frames.

    // Add checksum.
    constexpr std::size_t checksumOffset{8U};
    const std::uint16_t checksum{checksum16(buf, checksumOffset)};
    buf[8U] = static_cast<std::uint8_t>((checksum >> 8U) & 0xFFU);
    buf[9U] = static_cast<std::uint8_t>((checksum) & 0xFFU);

    // Return the number of serialized bytes.
    return frameLen;
}
} // namespace comm::frame

// -----------------------------------------------------------------------------
void printBytes(const std::uint8_t* bytes, const std::size_t byteCount, 
                std::ostream& ostream = std::cout) noexcept
{
    if ((nullptr == bytes) || (0U == byteCount)) { return; }
    const std::size_t last{byteCount - 1U};
    ostream << "[";

    for (std::size_t i{}; i < last; ++i)
    {
        ostream << std::hex << static_cast<int>(bytes[i]) << ", ";
    }
    ostream << std::hex << static_cast<int>(bytes[last]) << "]\n";
}
} // namespace

// -----------------------------------------------------------------------------
int main()
{
    constexpr std::size_t bufLen{20U};
    std::uint8_t buf[bufLen]{};
    
    constexpr std::uint8_t dstAddr{0x02U};
    constexpr std::uint8_t srcAddr{0x01U};
    constexpr std::uint16_t seqNr{0x20U};

    const std::size_t byteCount{comm::frame::serializePing(buf, bufLen, dstAddr, srcAddr, seqNr)};
    if (0U == byteCount) { return -1; }

    std::cout << "Serialized ping frame (" << byteCount << " bytes): ";
    printBytes(buf, byteCount);
    return 0;
}
```

#### Uppdaterat kodexempel

Med ett funktionstemplate `writeToBuf()` kan serialiseringen förenklas. I denna funktion är anroparen ansvarig för att buffern är tillräckligt stor, dvs. vi förväntar oss att anroparen
redan har kollat bufferstorleken:

```cpp
/**
 * @brief Write value to buffer.
 * 
 * @param[out] buffer The buffer to write to.
 * @param[in] offset Value offset, i.e. the index for which to write the value.
 * @param[in] value The value to write. Must be unsigned.
 */
template<typename T>
void writeToBuf(std::uint8_t* buffer, const std::size_t offset, const T value) noexcept
{
    static_assert(std::is_unsigned<T>::value, "Failed to write to buffer: T must be unsigned!");
    constexpr std::size_t len{sizeof(T)};
    constexpr std::size_t msb{len - 1U};
    constexpr std::size_t bitsPerByte{8U};

    for (std::size_t i{}; i < len; ++i)
    {
        const std::size_t shift{bitsPerByte * (msb - i)};
        const std::uint8_t byte{static_cast<std::uint8_t>((value >> shift) & 0xFFU)};
        buffer[offset + i] = byte;
    }
}
```

För läsbarhetens skull kan vi också lägga till struktar innehållande parametrar såsom fältlängder samt fältoffsets.

```cpp
/**
 * @brief Structure of field lengths.
 */
struct FieldLen
{
    /** Length of the SOF field. */
    static constexpr std::size_t Sof{2U};

    /** Length of the LEN field. */
    static constexpr std::size_t Len{1U};

    /** Length of the TYPE field. */
    static constexpr std::size_t Type{1U};

    /** Length of the DST field. */
    static constexpr std::size_t Dst{1U};

    /** Length of the SRC field. */
    static constexpr std::size_t Src{1U};

    /** Length of the SEQ field. */
    static constexpr std::size_t Seq{2U};

    /** Length of the checksum field. */
    static constexpr std::size_t Chk{2U};

    /** Header length. */
    static constexpr std::size_t Header{Sof + Len + Type + Dst + Src + Seq};

    /** Footer length. */
    static constexpr std::size_t Footer{Chk};

    /** Baseline length. */
    static constexpr std::size_t Baseline{Header + Footer};

    /** Maximal payload length. */
    static constexpr std::size_t MaxPayload{32U};

     /** Maximal frame length. */
    static constexpr std::size_t MaxFrame{Baseline + MaxPayload};
};

/**
 * @brief Structure of field offsets.
 */
struct FieldOffset
{
    /** Offset of the SOF field. */
    static constexpr std::size_t Sof{0U};

    /** Offset of the LEN field. */
    static constexpr std::size_t Len{Sof + FieldLen::Sof};

    /** Offset of the TYPE field. */
    static constexpr std::size_t Type{Len + FieldLen::Len};

    /** Offset of the DST field. */
    static constexpr std::size_t Dst{Type + FieldLen::Type};

    /** Offset of the SRC field. */
    static constexpr std::size_t Src{Dst + FieldLen::Dst};

    /** Offset of the SEQ field. */
    static constexpr std::size_t Seq{Src + FieldLen::Src};

    /** Offset of the DATA field. */
    static constexpr std::size_t Data{Seq + FieldLen::Seq};

    /**
     * @brief Get the offset of the CHK field.
     * 
     * @param[in] payloadLen Payload length, i.e. length of the DATA field.
     * 
     * @return Offset of the CHK field.
     */
    static constexpr std::size_t checksum(const std::size_t payloadLen) noexcept
    {
        return Data + payloadLen;
    }
};

/**
 * @brief Frame parameter structure.
 */
struct FrameParam
{
    /** Start-of-Field value. */
    static constexpr std::uint16_t Sof{0xA5F7U};
};
```

Serialiseringsfunktionen `serializePing()` kan då uppdateras till följande:

```cpp
// -----------------------------------------------------------------------------
std::size_t serializePing(std::uint8_t* buf, const std::size_t bufLen,
                          const std::uint8_t dstAddr, const std::uint8_t srcAddr,
                          const std::uint16_t seqNr) noexcept
{
    // No payload in PING -> frame length equals the baseline length.
    constexpr std::size_t payloadLen{0U};
    constexpr std::size_t frameLen{FieldLen::Baseline + payloadLen};
    if ((nullptr == buf) || (frameLen > bufLen)) { return 0U; }

    // Add header.
    writeToBuf(buf, FieldOffset::Sof, FrameParam::Sof);
    writeToBuf(buf, FieldOffset::Len, payloadLen);
    writeToBuf(buf, FieldOffset::Type, static_cast<std::uint8_t>(Type::Ping));
    writeToBuf(buf, FieldOffset::Dst, dstAddr);
    writeToBuf(buf, FieldOffset::Src, srcAddr);
    writeToBuf(buf, FieldOffset::Seq, seqNr);

    // Add footer.
    constexpr std::size_t checksumOffset{FieldOffset::checksum(payloadLen)};
    const std::uint16_t checksum{checksum16(buf, checksumOffset)};
    writeToBuf(buf, checksumOffset, checksum);

    // Return the number of serialized bytes.
    return frameLen;
}
```

---

### Deserialisering
Deserialisering innebär att tolka en mottagen byte‑array och extrahera strukturerad data:
* Först valideras fälten, exempelvis SOF, type och checksumma.
* Om fältet är giltigt extraheras datan ur fältet.

Detta är viktigt eftersom UART/RS‑485 levererar bytes i en ström, och vi kan få:
* Skräpdata mellan frames.
* Tappade bytes.
* Trasiga frames.

Nedan visas en funktion `deserialize()`, som deserialiserar en given frame:
* Vi kontrollerar att:
    * `SOF == 0xA5F7`.
    * Payload-längden inte överstiger satt maxvärde `(32 bytes)`.
    * Frame-typen är giltig.
    * Checksummen är korrekt.
    * Dessutom görs en check att den givna payload-buffern är tillräckligt stor för att
    rymma payloaden.
* Om samtliga checkar gick igenom extraheras datan.

```cpp
/**
 * @brief Deserialize data.
 * 
 *        Retrieve parameters if the given buffer contains a valid frame.
 * 
 * @param[in] buf Buffer to deserialize.
 * @param[in] bufLen Buffer length.
 * @param[out] payloadLen Retrieved payload length.
 * @param[out] frameType Retrieved frame type.
 * @param[out] dstAddr Retrieved destination address.
 * @param[out] srcAddr Retrieved source address.
 * @param[out] seqNr Retrieved sequence number.
 * @param[out] payloadBuf Retrieved payload (if any).
 * @param[in] payloadBufLen Length of the payload buffer.
 * 
 * @return True if the deserialization succeeded, false otherwise.
 */
bool deserialize(const std::uint8_t* buf, const std::size_t bufLen,
                 std::uint8_t& payloadLen, Type& frameType, std::uint8_t& dstAddr,
                 std::uint8_t& srcAddr, std::uint16_t& seqNr,
                 std::uint8_t* payloadBuf, const std::size_t payloadBufLen) noexcept
{
    // Terminate the function if the buffer is invalid or doesn't fit the header.
    if ((nullptr == buf) || (FieldLen::Baseline > bufLen)) { return false; }

    // Check SOF, return false if invalid.
    const std::uint16_t sof{static_cast<std::uint16_t>(
        (static_cast<std::uint16_t>(buf[FieldOffset::Sof]) << 8U) |
        (static_cast<std::uint16_t>(buf[FieldOffset::Sof + 1U])))};
    if (FrameParam::Sof != sof) { return false; }

    // Extract payload length, return false if invalid.
    const std::uint8_t len{buf[FieldOffset::Len]};
    if ((FieldLen::MaxPayload < len) || (bufLen < (FieldLen::Baseline + len))) { return false; }

    // Check the payload buffer, return false if invalid or too small.
    if ((0U < len) && (nullptr == payloadBuf)) { return false; }
    if (payloadBufLen < len) { return false; }

    // Extract frame type as a byte, return false if invalid.
    const std::uint8_t type{buf[FieldOffset::Type]};
    if (static_cast<std::uint8_t>(Type::Unknown) <= type) { return false; }

    // Compute the expected checksum.
    const std::size_t checksumOffset{FieldOffset::checksum(len)};
    const std::uint16_t expectedChecksum{checksum16(buf, checksumOffset)};

    // Extract checksum, return false if invalid.
    const std::uint16_t actualChecksum{static_cast<std::uint16_t>(
        (static_cast<std::uint16_t>(buf[checksumOffset]) << 8U) |
        (static_cast<std::uint16_t>(buf[checksumOffset + 1U])))};
    if (expectedChecksum != actualChecksum) { return false; }

    // Extract addresses and sequence number.
    const std::uint8_t dst{buf[FieldOffset::Dst]};
    const std::uint8_t src{buf[FieldOffset::Src]};
    const std::uint16_t seq{static_cast<std::uint16_t>(
        (static_cast<std::uint16_t>(buf[FieldOffset::Seq]) << 8U) |
        (static_cast<std::uint16_t>(buf[FieldOffset::Seq + 1U])))};

    // Store extracted parameters.
    payloadLen = len;
    frameType  = static_cast<Type>(type);
    dstAddr    = dst;
    srcAddr    = src;
    seqNr      = seq;

    // Retrieve payload.
    const std::size_t payloadOffset{FieldOffset::Data};

    for (std::size_t i{}; i < payloadLen; ++i)
    {
        payloadBuf[i] = buf[payloadOffset + i];
    }
    // Return true to indicate success.
    return true;
}
```

#### Uppdaterat kodexempel

Vi kan använda ett funktionstemplate `readFromBuf()` för att minska antalet bitoperationer som behövs:

```cpp
/**
 * @brief Read value from buffer.
 * 
 * @tparam T The value type. Must be unsigned.
 * 
 * @param[in] buf Buffer to read from.
 * @param[in] offset Value offset in buffer.
 * 
 * @return The retrieved value.
 */
template<typename T = std::uint8_t>
T readFromBuf(const std::uint8_t* buf, const std::size_t offset) noexcept
{
    static_assert(std::is_unsigned<T>::value, "Failed to read from buffer: T must be unsigned!");
    constexpr std::size_t len{sizeof(T)};
    constexpr std::size_t msb{len - 1U};
    constexpr std::size_t bitsPerByte{8U};
    T value{};

    for (std::size_t i{}; i < len; ++i)
    {
        const std::size_t shift{bitsPerByte * (msb - i)};
        value |= static_cast<T>(buf[offset + i]) << shift;
    }
    return value;
}
```

Genom att använda `readFromBuf()` kan funktionen `deserialize()` förenklas till:

```cpp
// -----------------------------------------------------------------------------
bool deserialize(const std::uint8_t* buf, const std::size_t bufLen,
                 std::uint8_t& payloadLen, Type& frameType, std::uint8_t& dstAddr,
                 std::uint8_t& srcAddr, std::uint16_t& seqNr,
                 std::uint8_t* payloadBuf, const std::size_t payloadBufLen) noexcept
{
    // Terminate the function if the buffer is invalid or doesn't fit the header.
    if ((nullptr == buf) || (FieldLen::Baseline > bufLen)) { return false; }

    // Check SOF, return false if invalid.
    const std::uint16_t sof{readFromBuf<std::uint16_t>(buf, FieldOffset::Sof)};
    if (FrameParam::Sof != sof) { return false; }

    // Extract payload length, return false if invalid.
    const std::uint8_t len{readFromBuf(buf, FieldOffset::Len)};
    if ((FieldLen::MaxPayload < len) || (bufLen < (FieldLen::Baseline + len))) { return false; }

    // Check the payload buffer, return false if invalid or too small.
    if ((0U < len) && (nullptr == payloadBuf)) { return false; }
    if (payloadBufLen < len) { return false; }

    // Extract frame type as a byte, return false if invalid.
    const std::uint8_t type{readFromBuf(buf, FieldOffset::Type)};
    if (static_cast<std::uint8_t>(Type::Unknown) <= type) { return false; }

    // Compute the expected checksum.
    const std::size_t checksumOffset{FieldOffset::checksum(len)};
    const std::uint16_t expectedChecksum{checksum16(buf, checksumOffset)};

    // Extract checksum, return false if invalid.
    const std::uint16_t actualChecksum{readFromBuf<std::uint16_t>(buf, checksumOffset)};
    if (expectedChecksum != actualChecksum) { return false; }

    // Extract addresses and sequence number.
    const std::uint8_t dst{readFromBuf(buf, FieldOffset::Dst)};
    const std::uint8_t src{readFromBuf(buf, FieldOffset::Src)};
    const std::uint16_t seq{readFromBuf<std::uint16_t>(buf, FieldOffset::Seq)};

    // Store extracted parameters.
    payloadLen = len;
    frameType  = static_cast<Type>(type);
    dstAddr    = dst;
    srcAddr    = src;
    seqNr      = seq;

    // Retrieve payload.
    const std::size_t payloadOffset{FieldOffset::Data};

    for (std::size_t i{}; i < payloadLen; ++i)
    {
        payloadBuf[i] = buf[payloadOffset + i];
    }
    // Return true to indicate success.
    return true;
}
```

### Exempelprogram - serialisering samt deserialisering

Nedan visas ett program, där en ping-frame serialiseras. Därefter deserialiseras datan och vi
skriver ut vad som extraherades:

```cpp
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace
{
namespace comm::frame
{
// Enumeration of frame types.
enum class Type : std::uint8_t
{
    Ping,
    Pong,
    StatusRequest,
    StatusResponse,
    Unknown,
};

// Structure of field lengths.
struct FieldLen
{
    static constexpr std::size_t Sof{2U};
    static constexpr std::size_t Len{1U};
    static constexpr std::size_t Type{1U};
    static constexpr std::size_t Dst{1U};
    static constexpr std::size_t Src{1U};
    static constexpr std::size_t Seq{2U};
    static constexpr std::size_t Chk{2U};
    static constexpr std::size_t Header{Sof + Len + Type + Dst + Src + Seq};
    static constexpr std::size_t Footer{Chk};
    static constexpr std::size_t Baseline{Header + Footer};
    static constexpr std::size_t MaxPayload{32U};
    static constexpr std::size_t MaxFrame{Baseline + MaxPayload};
};

// Structure of field offsets.
struct FieldOffset
{
    static constexpr std::size_t Sof{0U};
    static constexpr std::size_t Len{Sof + FieldLen::Sof};
    static constexpr std::size_t Type{Len + FieldLen::Len};
    static constexpr std::size_t Dst{Type + FieldLen::Type};
    static constexpr std::size_t Src{Dst + FieldLen::Dst};
    static constexpr std::size_t Seq{Src + FieldLen::Src};
    static constexpr std::size_t Data{Seq + FieldLen::Seq};

    // -----------------------------------------------------------------------------
    static constexpr std::size_t checksum(const std::size_t payloadLen) noexcept
    { 
        return Data + payloadLen;
    }
};

/**
 * @brief Frame parameter structure.
 */
struct FrameParam
{
    /** Start-of-Field value. */
    static constexpr std::uint16_t Sof{0xA5F7U};
};

// -----------------------------------------------------------------------------
template<typename T>
void writeToBuf(std::uint8_t* buf, const std::size_t offset, const T value) noexcept
{
    static_assert(std::is_unsigned<T>::value, "Failed to write to buffer: T must be unsigned!");
    constexpr std::size_t len{sizeof(value)};
    constexpr std::size_t msb{len - 1U};
    constexpr std::size_t bitsPerByte{8U};

    for (std::size_t i{}; i < len; ++i)
    {
        const std::size_t shift{bitsPerByte * (msb - i)};
        const std::uint8_t byte{static_cast<std::uint8_t>((value >> shift) & 0xFFU)};
        buf[offset + i] = byte;
    }
}

// -----------------------------------------------------------------------------
template<typename T = std::uint8_t>
T readFromBuf(const std::uint8_t* buf, const std::size_t offset) noexcept
{
    static_assert(std::is_unsigned<T>::value, "Failed to read from buffer: T must be unsigned!");
    constexpr std::size_t len{sizeof(T)};
    constexpr std::size_t msb{len - 1U};
    constexpr std::size_t bitsPerByte{8U};
    T value{};

    for (std::size_t i{}; i < len; ++i)
    {
        const std::size_t shift{bitsPerByte * (msb - i)};
        value |= static_cast<T>(buf[offset + i]) << shift;
    }
    return value;
}

// -----------------------------------------------------------------------------
std::uint16_t checksum16(const std::uint8_t* data, const std::size_t dataLen) noexcept
{
    if ((nullptr == data) || (0U == dataLen)) { return 0U; }
    std::uint16_t checksum{};
    for (std::size_t i{}; i < dataLen; ++i) { checksum += data[i]; }
    return checksum;
}

// -----------------------------------------------------------------------------
std::size_t serializePing(std::uint8_t* buf, const std::size_t bufLen,
                          const std::uint8_t dstAddr, const std::uint8_t srcAddr,
                          const std::uint16_t seqNr) noexcept
{
    // No payload in PING -> frame length equals the baseline length.
    constexpr std::size_t payloadLen{0U};
    constexpr std::size_t frameLen{FieldLen::Baseline + payloadLen};
    if ((nullptr == buf) || (frameLen > bufLen)) { return 0U; }

    // Add header.
    writeToBuf(buf, FieldOffset::Sof, FrameParam::Sof);
    writeToBuf(buf, FieldOffset::Len, payloadLen);
    writeToBuf(buf, FieldOffset::Type, static_cast<std::uint8_t>(Type::Ping));
    writeToBuf(buf, FieldOffset::Dst, dstAddr);
    writeToBuf(buf, FieldOffset::Src, srcAddr);
    writeToBuf(buf, FieldOffset::Seq, seqNr);

    // Add footer.
    constexpr std::size_t checksumOffset{FieldOffset::checksum(payloadLen)};
    const std::uint16_t checksum{checksum16(buf, checksumOffset)};
    writeToBuf(buf, checksumOffset, checksum);

    // Return the number of serialized bytes.
    return frameLen;
}

// -----------------------------------------------------------------------------
bool deserialize(const std::uint8_t* buf, const std::size_t bufLen,
                 std::uint8_t& payloadLen, Type& frameType, std::uint8_t& dstAddr,
                 std::uint8_t& srcAddr, std::uint16_t& seqNr,
                 std::uint8_t* payloadBuf, const std::size_t payloadBufLen) noexcept
{
    // Terminate the function if the buffer is invalid or doesn't fit the header.
    if ((nullptr == buf) || (FieldLen::Baseline > bufLen)) { return false; }

    // Check SOF, return false if invalid.
    const std::uint16_t sof{readFromBuf<std::uint16_t>(buf, FieldOffset::Sof)};
    if (FrameParam::Sof != sof) { return false; }

    // Extract payload length, return false if invalid.
    const std::uint8_t len{readFromBuf(buf, FieldOffset::Len)};
    if ((FieldLen::MaxPayload < len) || (bufLen < (FieldLen::Baseline + len))) { return false; }

    // Check the payload buffer, return false if invalid or too small.
    if ((0U < len) && (nullptr == payloadBuf)) { return false; }
    if (payloadBufLen < len) { return false; }

    // Extract frame type as a byte, return false if invalid.
    const std::uint8_t type{readFromBuf(buf, FieldOffset::Type)};
    if (static_cast<std::uint8_t>(Type::Unknown) <= type) { return false; }

    // Compute the expected checksum.
    const std::size_t checksumOffset{FieldOffset::checksum(len)};
    const std::uint16_t expectedChecksum{checksum16(buf, checksumOffset)};

    // Extract checksum, return false if invalid.
    const std::uint16_t actualChecksum{readFromBuf<std::uint16_t>(buf, checksumOffset)};
    if (expectedChecksum != actualChecksum) { return false; }

     // Extract addresses and sequence.
    const std::uint8_t dst{readFromBuf(buf, FieldOffset::Dst)};
    const std::uint8_t src{readFromBuf(buf, FieldOffset::Src)};
    const std::uint16_t seq{readFromBuf<std::uint16_t>(buf, FieldOffset::Seq)};

    // Store extracted parameters.
    payloadLen = len;
    frameType  = static_cast<Type>(type);
    dstAddr    = dst;
    srcAddr    = src;
    seqNr      = seq;

    // Retrieve payload.
    const std::size_t payloadOffset{FieldOffset::Data};

    for (std::size_t i{}; i < payloadLen; ++i)
    {
        payloadBuf[i] = buf[payloadOffset + i];
    }
    // Return true to indicate success.
    return true;
}
} // namespace comm::frame

// -----------------------------------------------------------------------------
void printBytes(const std::uint8_t* bytes, const std::size_t byteCount, 
                std::ostream& ostream = std::cout) noexcept
{
    if ((nullptr == bytes) || (0U == byteCount)) { return; }
    const std::size_t last{byteCount - 1U};
    ostream << "[";

    for (std::size_t i{}; i < last; ++i)
    {
        ostream << std::hex << static_cast<int>(bytes[i]) << ", ";
    }
    ostream << std::hex << static_cast<int>(bytes[last]) << "]\n";
}
} // namespace

// -----------------------------------------------------------------------------
int main()
{
    constexpr std::size_t bufLen{20U};
    std::uint8_t buf[bufLen]{};
    
    // Serialize data.
    {
        constexpr std::uint8_t dstAddr{0x02U};
        constexpr std::uint8_t srcAddr{0x01U};
        constexpr std::uint16_t seqNr{0x20U};

        const std::size_t byteCount{
            comm::frame::serializePing(buf, bufLen, dstAddr, srcAddr, seqNr)};
        if (0U == byteCount) { return -1; }

        std::cout << "Serialized ping frame (" << byteCount << " bytes): ";
        printBytes(buf, byteCount);
    }

    // Deserialize data.
    {
        std::uint8_t payloadBuf[bufLen]{};
        std::uint8_t payloadLen{}, dstAddr{}, srcAddr{};
        std::uint16_t seqNr{};
        comm::frame::Type frameType{comm::frame::Type::Unknown};

        if (!comm::frame::deserialize(buf, bufLen, payloadLen, frameType, dstAddr, srcAddr, seqNr,
            payloadBuf, bufLen)) { return -1; }

        std::cout << "Deserialized frame:\n";
        std::cout << "\t- Frame type ID: " << static_cast<int>(frameType) << "\n";
        std::cout << "\t- Destination address: " << static_cast<int>(dstAddr) << "\n";
        std::cout << "\t- Source address: " << static_cast<int>(srcAddr) << "\n";
        std::cout << "\t- Sequence: " << static_cast<int>(seqNr) << "\n";
        
        if (0U < payloadLen)
        {
            std::cout << "\t- Payload (" << payloadLen << ") bytes: ";
            printBytes(payloadBuf, payloadLen);
        }
    }
    return 0;
}
```

---

## Sammanfattning
En frame är:
* Strukturerad data (header + payload).
* Serialiserbar till `std::uint8_t[]`.
* Deserialiserbar från `std::uint8_t[]`.
* Validerbar via `SOF`, `LEN` och `CHK`.

I embedded‑system är detta centralt eftersom:
* Kommunikation sker i form av råa byte‑strömmar.
* RAM/flash är begränsat.
* Robusthet är kritiskt.

---
