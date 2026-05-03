# Bilaga B

## Övningsuppgifter
Vi bygger en simulerad miljö med:
* En buss (`comm::bus::Interface` samt `comm::bus::Stub`).
* Noder (`comm::node::Interface` samt `comm::node::Stub`).
* Varje nod använder:
    * `comm::frame::Parser`.
    * `comm::frame::Frame`.
    * `handleFrame()` från testprogrammet i L02.

---

### **1.** Skapa buss-interface
Implementera följande interface `comm::bus::Interface` för databussen i en ny fil 
`comm/bus/interface.h`:

```cpp
/**
 * @brief Bus interface.
 */
#pragma once

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

    /**
     * @brief Reset the bus and all connected nodes.
     */
    //! @todo Name the method `reset()`.
};
} // namespace comm::bus
```

Funktion:
* `addNode()` registrerar en nod på bussen.
* `sendByte()` lägger en byte i bussens interna kö.
* `tick()` levererar bytes till noderna.
* `reset()` återställer bussen och samtliga anslutna noder.

---

### **2.** Skapa nod-interface
Implementera följande interface `comm::node::Interface` för en nod i en ny fil `comm/node/interface.h`:

```cpp
/**
 * @brief Node interface.
 */
#pragma once

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

    /**
     * @brief Reset the node.
     */
    //! @todo Name the method `reset()`.
};
} // namespace comm::node
```

Funktion:
* `address()` ger nodens address.
* `onReceive()` tar emot bytes från bussen (broadcast).
* `tick()` driver nodens logik utan blockering.
* `reset()` återställer nodens interna tillstånd.

---

### **3.** Implementera `comm::bus::Stub`
Implementera en buss-stub i en ny fil `comm/bus/stub.h`:

Bussen ska följande interna konstanter (`static constexpr`):
* `MaxNodeCount` = `10U`
* `MaxQueuedBytes` = `20U`

Bussen ska följande medlemsvariabler:
* `myNodes[MaxNodeCount]`:
    * Array med pekare till anslutna noder.
* `myQueue[MaxQueuedBytes]`:
    * Intern kö som lagrar data (bytes) som ska skickas till alla noder.
    * Ska arbeta efter FIFO-principen (*First In First Out*), dvs. den äldsta byten ska
    poppas/tas ut först.
* `myNodeCount`:
    * Antalet anslutna noder
* `myQueueLen`:
    * Antalet bytes i den interna kön.
* `myFirstByteIndex`:
    * Index för den första (äldsta) byten i den interna kön.
* `myNextByteIndex`:
    * Index där nästa byte ska placeras i den interna kön.

Implementera följande metoder i en ny fil `comm/bus/stub.cpp`:
* `sendByte()`:
    * Given byte läggs i den intern kön `myQueue` om utrymme finns.
    * Använd `myNextByteIndex` för att placera den nya byten längst bak i kön.
    * Inkrementera `myNextByteIndex` efter skrivningen och se till att dena hålls inom bufferns storlek:  
    `myNextByteIndex = myNextByteIndex % MaxQueuedBytes`.
    * `True` ska returneras om given byte lades till ska `true` annars `false`.
* `tick()`:
    * Om kön innehåller data:
        * Poppa en byte enligt FIFO-principen, dvs. den äldsta byten ska poppas.
        * Använd `myFirstByteIndex` för att läsa ut den äldsta byten.
        * Inkrementera `myFirstByteIndex` efter läsningen och se till att det hålls inom 
        bufferns storlek:  
        `myFirstByteIndex = myFirstByteIndex % MaxQueuedBytes`.
        * Leverera byten till alla noder genom att anropa `node.onReceive(byte)`.
* `reset()`:
    * Nollställer samtliga medlemsvariabler.
    * Resettar samtliga anslutna noder via `myNodes[i]->reset()`.

---

### **4.** Implementera `comm::node::Stub`
Implementera en nod-stub i en ny fil `comm/node/stub.h`:
* Har en frame-parser `myParser`.
* Har en referens till ett buss-interface `myBus`.
* Har ett 16-bitars sensorvärde `myVal`.
* Har en 8-bitars adress `myAddr`.

Implementera följande metoder i en ny fil `comm/node/stub.cpp`:
* Konstruktorn:
    * Tar emot en address, en buss-referens samt ett sensorvärde.
    * Som default ska sensorvärdet sättas till `0`.
    * Sparar given address i `myAddr`.
    * Sparar given buss-referens i `myBus`.
    * Sparar givet sensorvärde i `myVal`.
    * Kopplar noden till bussen (`myBus.addNode(*this)`).
* `address()`:
    * Returnerar `myAddr`.
* `onReceive(std::uint8_t byte)`:
    * Matar byte till frame-parsern.
* `tick()`:
    * Om frame-parsern har en komplett frame:
        * Extrahera framen, avsluta metoden vid fel.
        * Om `frame.dstAddr != myAddr` → ignorera.
        * Annars:
            * Resetta parsern.
            * Anropa metoden [handleFrame()](./code/node/stub.cpp) för att svara på framen.
            * Om `handleFrame()` returnerar `true`:
                * Serialisera framen.
                * Resetta bussen via `myBus.reset()`.
                * Skicka bytes via `myBus.sendByte()`.
            * Annars, gör ingenting.
* `reset()`:
    * Resettar den interna parsern.
* `handleFrame()`:
    * Privat metod, implementerad i `./code/node/stub.cpp`, som kan klistras in direkt.
    * Metoden vidareutvecklar och ersätter motsvarande metod från L02.
    * Filerna `comm/frame/handler.h` och `comm/frame/handler.cpp` kan därför tas bort.

---

### **5.** Validera implementationen
Kompilera och kör testprogrammet i [main.cpp](./code/main.cpp) i en Linuxterminal:

```bash
make
```

Det som sker i testet är att:
* Tre noder skapas på adressen `1-3`.
* Noderna ansluts till en databuss.
* En status request skickas från nod 1 till nod 2.
* Nod 2 svarar med ett status response innehållande dess sensorvärde.

---
