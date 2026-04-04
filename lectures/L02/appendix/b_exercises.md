# Bilaga B

## Övningsuppgifter – Byte-parsing

### **1.** Implementera en frame-parser
Implementera `comm::frame::Parser` med följande bas:

``` cpp
namespace comm::frame
{
class Parser
{
public:
    bool isFrameReady() const noexcept;
    bool processByte(std::uint8_t byte) noexcept;
    bool extractFrame(Frame& frame) noexcept;
    void reset() noexcept;

private:
    enum class State : std::uint8_t
    {
        WaitForSof1,
        WaitForSof2,
        WaitForLen,
        WaitForHeader,
        WaitForPayload,
        WaitForChecksum,
        Ready,
    };

    Frame& myFrame;
    std::uint8_t myParsedBytes;
    State myState;
};
} // namespace comm::frame
```

Parsern ska:
* Vänta på SOF byte 1.
* Vänta på SOF byte 2.
* Läsa LEN.
* Samla bytes tills hela framen är mottagen.
* Validera framen via `deserialize()`.
* Markera frame ready.

---

### **2.** Skicka PONG vid mottagande av PING
Implementera en funktion med namnet `handleFrame()`:

``` cpp
bool handleFrame(const Frame& txFrame, Frame& rxFrame) noexcept;
```

Denna funktion ska läsa `txFrame` och skicka returdata via `rxFrame`:
* Om TYPE == PING:
    * Byt DST/SRC.
    * Behåll SEQ.
    * TYPE = PONG.
    * LEN = 0 (payload tom).
    * Returnera `true`.
* Annars:
    * Returnera `false`.

---

### **3.** Sammankoppling
I `main()`:
* 1. Skapa en PING-frame. 
* 2. Serialisera framen. 
* 3. Mata in bytes en och en i Parser. 
* 4. När frame har extraherats:
    * Anropa handleFrame().
    * Serialisera PONG.
    * Skriv ut resultat på hexadecimal form.

#### Testfall
1.  Korrekt PING → korrekt PONG.
2.  Fel checksum → ska avvisas.
3.  Skräpdata före frame → parser ska återhämta sig.
4.  Två frames back-to-back.

---

### **4.** Robust SOF-hantering
Verifiera följande scenario:
* En byte `0xA5` (SOF1) tas emot.
* Nästa byte är inte `0xF7` (SOF2), utan exempelvis `0xA5` igen.
* Parsern ska kunna hantera detta utan att missa en potentiell frame-start.

---

### **5.** Reset-policy
När ska parsern göra `reset()`?
* Efter `extractFrame()`?
* Efter en trasig checksumma?
* Efter felaktig LEN?
* Efter fel SOF?

Skriv ner en policy i 3-5 punkter och motivera den kort.

---
