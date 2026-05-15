
# Bilaga B

## Övningsuppgifter
Vi bygger vidare på samma simulering som L03–L04:
* Bus: Broadcast, tick-driven, deterministisk felmodell.
* Node: Parser + routing.
* Nytt: Kvittenslogik.

### **1.** Lägg till ACK och NACK i `comm::frame::Type`
Uppdatera `comm::frame::Type` med följande frametyper:
* Lägg till `Ack`.
* Lägg till `Nack`.

Verifiera:
* `isTypeValid()` (om ni har en sådan) fungerar även efter utökning.
* `Unknown` ska fortfarande vara sista värdet.

---

### **2.** Skapa funktioner för att bygga ACK/NACK
Skapa två funktioner i exempelvis `comm/transport/utils.h`:

```cpp
bool makeAck(const comm::frame::Frame& rxFrame, comm::frame::Frame& txFrame) noexcept;
bool makeNack(const comm::frame::Frame& rxFrame, comm::frame::Frame& txFrame) noexcept;
```

Regler:
* `DST/SRC` ska swappas jämfört med `rxFrame`.
* `SEQ` ska vara samma som i `rxFrame`.
* `LEN = 0U`.
* `TYPE = Ack` eller `TYPE = Nack`.

---

### **3.** Skapa ett interface för transportlagret
Implementera följande interface:

```cpp
#include <cstdint>

/** Frame implementation. */
namespace comm::frame { class Frame; }

namespace comm::transport
{
/**
 * @brief Transport interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */

    /**
     * @brief Execute one tick of transport logic.
     */
    //! @todo Name the method 'tick()'.

    /**
     * @brief Provide a received application frame to the transport.
     * 
     *        The transport may generate ACK/NACK as a result.
     * 
     * @param[in] frame Received frame.
     */
    //! @todo Name the method 'onRxFrame()'.

    /**
     * @brief Check whether transport has a frame to transmit.
     * 
     * @return True if a TX frame is available, false otherwise.
     */
    //! @todo Name the method 'hasTxFrame()'.

    /**
     * @brief Pop next TX frame from transport.
     * 
     * @param[out] out Retrieved TX frame.
     * 
     * @return True if a frame was popped, false otherwise.
     */
    //! @todo Name the method 'popTxFrame()'.
};
} // namespace comm::transport
```

---

### **4.** Implementera `comm::transport::Stub`
Implementera en stubb med följande minimibeteende:
* När `onRxFrame()` anropas:
    * Om frame är en applikationsframe och är adresserad till noden:
        * Skapa och köa ett ACK.
* Vid fel i frame:
    * Skapa och köa ett NACK om ni kan (se [bilaga A](./a_ack_nack.md)).
* `hasTxFrame()` och `popTxFrame()` ska hantera en enkel FIFO-kö av frames.

**Notera:**
* I L05 behöver transporten inte vänta på ACK.
* Den ska endast generera kvittens.

---

### **5.** Uppdatera `comm::node::Stub`
Uppdatera nodstubbens `tick()`:
* När en frame extraherats och noden är den avsedda mottagaren:
    * Ge framen till transporten via `transport.onRxFrame(frame)`.
    * Hantera applikationslogik (PING → PONG) som tidigare.

* Efter att noden hanterat RX:
    * Om `transport.hasTxFrame()`:
        * Poppa framen, serialisera och skicka bytes via `bus.sendByte(...)`.

---

### **6.** Demonstrera ACK/NACK med deterministiska fel
Skapa minst tre scenarion:

1) Inga fel:
* PING från A till B.
* B skickar PONG.
* B skickar ACK på PING.

2) Korrupt byte (L04):
* Korrupta en byte i PING.
* B ska inte leverera PING till applikationslogik.
* (Om möjligt) B skickar NACK.

3) Drop (L04):
* Droppa hela PING-framen (eller en byte så att framen aldrig blir valid).
* Ingen ACK/NACK kommer fram.
* Diskutera: detta leder till timeout/retry i L06.

---

## Reflektion inför L06
Diskutera:
* Vad gör sändaren om ACK uteblir?
* Hur länge ska sändaren vänta?
* Vad händer om sändaren skickar om och mottagaren får en dubblett?

---
