# Bilaga A

## Frame-parsing
I verkliga system (UART, RS-485, osv.) kommer data som en byte i taget. Det innebär att vi:
* Inte vet var en frame börjar.
* Inte vet om första byten vi ser tillhör en frame eller är skräp.
* Inte vet hur lång framen är förrän vi har läst LEN.
* Måste kunna återhämta oss om en frame är trasig (fel checksumma, tappad byte, osv.).

En **parser** löser detta genom att:
* Läsa bytes en i taget.
* Leta efter SOF.
* Läsa LEN och därmed veta hur många bytes som återstår.
* Ackumulera bytes tills en komplett frame är befintlig.
* Validera framen (via `Frame::deserialize()`).
* Markera att en frame är redo att hämtas.

---

### Designmål
Parsern ska vara:
* Deterministisk (ingen blockering).
* Robust (återhämtar sig från skräpdata).
* Enkel att testa (mata in bytes i valfri ordning och verifiera resultat).
* Heap-fri (användning av statisk buffer internt).

---

### State machine
Parsern ska implementeras som en state machine med följande tillstånd:

```cpp
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
```

Idén är att följande data ska läsas in i respektive tillstånd:
* `WaitForSof1`: Den första SOF-byten (t.ex. `0xA5`).
* `WaitForSof2`: Den andra SOF-byten (t.ex. `0xF7`).
* `WaitForLen`: Payloadlängden (LEN).
* `WaitForType`: Frame-typen (TYPE).
* `WaitForDst`: Destinationsaddressen (DST).
* `WaitForSrc`: Avsändaradressen (SRC).
* `WaitForSeq1`: Den första sekvensnummber-byten (SEQ).
* `WaitForSeq1`: Den andra sekvensnummber-byten (SEQ).
* `WaitForPayload`: LEN bytes payload (DATA).
* `WaitForChk1`: Den första checksumme-byten (CHK).
* `WaitForChk2`: Den andra checksumme-byten (CHK).
* `Ready`: En komplett frame finns i den interna buffern.

---

### Buffering och validering
En vanlig strategi:
* Parsern har en intern `std::uint8_t myBuf[MaxFrameLen]`.
* Varje mottagen byte läggs i buffern på rätt plats.
* När hela framen är mottagen:
    * `Frame::deserialize(myBuf, frameLen)` anropas.
    * Om deserialisering lyckas:
        * `Ready` sätts.
    * Annars:
        * Parsern gör `reset()` och fortsätter leta efter SOF.

**OBS!**
* Om skräpdata kommer före SOF ska parsern ignorera detta tills SOF hittas.
* Om SOF byte 1 matchar men byte 2 inte matchar ska parsern återställas till starttillståndet.

---

### Rekommenderat beteende för API

#### `isFrameReady()`
* Returnerar `true` om parsern är i tillståndet `Ready`, annars `false`.

#### `processByte(std::uint8_t byte)`
* Tar emot en byte.
* Uppdaterar tillståndsmaskinen.
* Returnerar `true` om given byte parsades, annars `false`.

#### `extractFrame(Frame& frame)`
* Om `isFrameReady()`:
    * Returnera `false` direkt.
* Annars:
    * Deserialisera framen via `frame`.
    * Sätt parsern tillbaka till lämpligt startläge (typiskt `reset()`).
    * Returnera `true` om deserialiseringen lyckades, annars `false`.

#### `reset()`
* Återställ state, räknare och bufferindex så att parsern kan leta efter nästa frame.

---
