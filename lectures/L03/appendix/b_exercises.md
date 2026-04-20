# Bilaga B

## Övningsuppgifter
Vi bygger en simulerad miljö med:
* En buss (`comm::bus::Interface` + `comm::bus::Stub`).
* Noder (`comm::node::Interface` + `comm::node::Stub`).
* Varje nod använder:
    * `comm::frame::Parser`.
    * `comm::frame::Frame`.
    * `handleFrame()` från L02 (PING → PONG).

---

### **1.** Skapa buss-interface
Implementera följande interface `comm::bus::Interface` för databussen i en ny fil 
`comm/bus/interface.h`:

```cpp
#include <cstdint>

namespace comm
{
/** Node interface. */
namespace node { class Interface; }
} // namespace comm

namespace comm::bus
{
/**
 * @brief Bus interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */

    /**
     * @brief Add node to the bus.
     * 
     * @param[in] node Node to add.
     * 
     * @return True if the node was added, false otherwise.
     */
    //! @todo Name the method 'addNode()'.

    /**
     * @brief Send byte on the bus (broadcast).
     *
     *        The byte is delivered to all registered nodes.
     *        Routing is performed in the nodes, not in the bus.
     *
     * @param[in] byte Byte to send.
     *
     * @return True if the byte was queued successfully, false otherwise.
     */
    //! @todo Name the method 'sendByte()'.

    /**
     * @brief Increment the clock.
     * 
     *        Deliver pending bytes to all nodes.
     */
    //! @todo Name the method 'tick()'.
};
} // namespace comm::bus
```

Funktion:
* `addNode()` registrerar en nod på bussen.
* `sendByte()` lägger en byte i bussens interna kö.
* `tick()` levererar bytes till noderna.

---

### **2.** Skapa nod-interface
Implementera följande interface `comm::node::Interface` för en nod i en ny fil `comm/node/interface.h`:

```cpp
#include <cstdint>

namespace comm::node
{
/**
 * @brief Node interface.
 */
class Interface
{
public:
    /**
     * @brief Destructor.
     */

    /**
     * @brief Get the node address.
     * 
     * @return Node address.
     */
    //! @todo Name the method 'address()'.

    /**
     * @brief Receive byte from the bus.
     * 
     *        This method is invoked by the bus when a byte is delivered.
     *        The node is responsible for feeding the byte into its parser.
     * 
     * @param[in] byte Received byte.
     */
    //! @todo Name the method 'onReceive()'.

    /**
     * @brief Execute one tick of node logic.
     * 
     *        The node should run its parser and handle complete frames.
     *        Any required transmissions should be queued on the bus.
     */
    //! @todo Name the method 'tick()'.
};
} // namespace comm::node
```

Funktion:
* `address()` används av noden (och ev. bussen) för debug/test.
* `onReceive()` tar emot bytes från bussen (broadcast).
* `tick()` driver nodens logik utan blockering.

---

### **3.** Implementera `comm::bus::Stub`
Implementera en stubb-buss med följande beteende:
* Bussen har en lista med noder (referenser).
* När `sendByte()` anropas:
    * Byten läggs i en intern kö (FIFO, dvs. *First In First Out*).
* När `tick()` anropas:
    * Om kön innehåller data:
        * Poppa en byte (FIFO).
        * Leverera den till alla noder genom att anropa `node.onReceive(byte)`.

---

### **4.** Implementera `comm::node::Stub`
Skapa en nodstubb som:
* Har en adress (`std::uint8_t myAddr`).
* Har en `comm::frame::Parser`.
* Har en RX-frame och TX-frame.
* Har en referens till `comm::bus::Interface`.

Implementera följande metoder:
* `address()`:
    * Returnerar `myAddr`.
* `onReceive(std::uint8_t byte)`:
    * Matar byte till frame-parsern.
* `tick()`:
    * Om frame-parsern har en komplett frame:
        * Extrahera frame.
        * Om `frame.dst != myAddr` → ignorera.
        * Annars:
            * Anropa `handleFrame()` (PING → PONG).
            * Om `handleFrame()` returnerar `true`:
                * Serialisera PONG och skicka bytes via `bus.sendByte(...)`.

---

### **5.** Skapa tre noder
Skapa följande i `main()`:
* `node1` (adress `0x01`).
* `node2` (adress `0x02`).
* `node3` (adress `0x25`).

Registrera alla noder på bussen via `bus.addNode(...)`.

---

### **6.** Skicka PING från A till B
* Skapa en PING-frame i `node1`.
* DST = `0x02`, SRC = `0x01`, SEQ valfritt.
* Serialisera och skicka bytes via bussen.

Kör en "simuleringsloop":

```cpp
for (...) 
{ 
    node1.tick(); 
    node2.tick(); 
    node3.tick(); 
    bus.tick(); 
}
```

Verifiera att:
* `node2` skickar PONG tillbaka till `node1`.
* `node3` ignorerar PING (fel destinationsadress).
* `node1` tar emot PONG från `node2`.

---

## Testfall
1.  PING från `node1` till `node2` → PONG tillbaka till `node1`.
2.  `node3` ignorerar frames där `DST != 0x25`.
3.  Två frames back-to-back: PING följt av PING.

---

## Notering om L04
I L04 bygger vi vidare på samma buss och noder:
* Vi inför en deterministisk felmodell (drop/korrupt/fördröjning).
* Vi introducerar ACK/NACK och börjar koppla ihop robusthet med bussens beteende.
* Ni behöver därför spara och kunna återanvända er L03-implementation.

---
