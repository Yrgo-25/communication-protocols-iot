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
* Heap-fri (fast buffer internt eller återanvändning av `Frame`/buffer).

---

### State machine
Parsern ska implementeras som en state machine med följande tillstånd:

```cpp
/**
 * @brief Enumeration of parser states.
 */
enum class State : std::uint8_t
{
    WaitForSof1,     ///< Waiting for the first SOF byte.
    WaitForSof2,     ///< Waiting for the second SOF byte.
    WaitForLen,      ///< Waiting for the payload length (LEN).
    WaitForHeader,   ///< Waiting for the remaining header fields (TYPE, DST, SRC, SEQ).
    WaitForPayload,  ///< Waiting for the payload (DATA).
    WaitForChecksum, ///< Waiting for the checksum (CHK).
    Ready,           ///< The frame is complete/ready.
};
```

Idén är att följande data ska läsas in i respektive tillstånd:
* `WaitForSof1`: Den första SOF-byten (t.ex. `0xA5`).
* `WaitForSof2`: Den andra SOF-byten (t.ex. `0xF7`).
* `WaitForLen`: Payloadlängden (LEN).
* `WaitForHeader`: Resterande headerfält (TYPE, DST, SRC, SEQ).
* `WaitForPayload`: LEN bytes payload (DATA).
* `WaitForChecksum`: Checksum (CHK).
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

**Viktigt:**
* Om skräpdata kommer före SOF ska parsern ignorera detta tills SOF hittas.
* Om SOF byte 1 matchar men byte 2 inte matchar:
    * Parsern ska inte fastna.
    * Den ska återgå till ett tillstånd där den kan hitta SOF igen.

---

### Rekommenderat beteende för API

#### `processByte(std::uint8_t byte)`
* Tar emot en byte.
* Uppdaterar state machine.
* Returnerar `true` om en frame blev komplett i samband med detta byte (dvs. tillstånd `Ready` nåddes), annars `false`.

#### `isFrameReady()`
* Returnerar `true` om parsern är i `Ready`.

#### `extractFrame(Frame& frame)`
* Om `isFrameReady()`:
    * Kopiera/assigna ut framen till argumentet `frame`.
    * Sätt parsern tillbaka till lämpligt startläge (typiskt `reset()`).
    * Returnera `true`.
* Om inte `Ready`:
    * Returnera `false`.

#### `reset()`
* Återställ state, räknare och bufferindex så att parsern kan leta efter nästa frame.

---

### Vanliga buggar
* FEL: Endian vid tolkning av 16-bitfält (SOF, SEQ, CHK).
* FEL: LEN tolkas som total längd (den räknar endast payload).
* FEL: Parsern fastnar om SOF2 är fel.
* FEL: Parsern accepterar en frame innan alla bytes har tagits emot.
* FEL: Parsern glömmer att återställas efter `extractFrame()`.

---