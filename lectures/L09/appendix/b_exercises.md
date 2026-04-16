# Bilaga B

## Övningsuppgifter

### 1 - Skapa en CAN-katalog
Skapa följande katalogstruktur:

```text
include/
    can/
        bus.h
        def.h
        frame.h
        master.h
        node.h
source/
    main.cpp
Makefile
```

Alla metoder ska implementeras inline i headerfilerna för enkelhets skull.

---

### 2 - Lägg till CAN-konstanter
I filen `can/def.h`, definiera följande konstanter (`static constexpr`) i namnrymden `can`:
* `RequestOffset`  = `0x300U`
* `ResponseOffset` = `0x400U`

---

### 3 – Skapa en CAN-frame
I filen `can/frame.h`, implementera en enkel datastruktur som representerar en CAN-frame via en 
strukt `can::Frame` med:
* Konstanter (`static constexpr`):
    * `IdMin`   = `0U`
    * `IdMax`   = `0x07FFU`
    * `DataLen` = `8U`
* Medlemsvariabler:
    * `data`: Ett datafält med plats för `DataLen` byte.
    * `id`: Ett 11-bitars CAN-ID (`IdMin-IdMax`). Ska markeras `const`.
    * `dlc`: Datalängden (`0U-DataLen`).

Lägg till följande metoder:
* Konstruktorn:
    * Tar emot ett CAN-ID.
    * Lagrar givet CAN-ID i `id`.
    * Sätter `dlc = 0`.
    * Nollställer datafältet.
    * Ska markeras `explicit` och `noexcept`.
* `isValid()`:
    * Returnerar `true` om CAN-ID:t är giltigt, annars `false`.
    * Ska markeras `const` och `noexcept`.
* `print()`:
    * Skriver ut ID.
    * Skriver ut DLC.
    * Skriver ut data.
    * Ska markeras `const` och `noexcept`.

Exempelutskrift:

```text
CAN ID: 0x255
DLC: 2
Data: 0x10 0x2F
```

---

### 4 – Skapa en nod
I filen `can/node.h`, implementera en klass döpt `can::Node`.

Klassen ska innehålla följande publika konstanter (`static constexpr`):
* `IdMin` = `0U`
* `IdMax` = `0x03FFU`

Klassen ska innehålla följande medlemsvariabler:
* `myId`: Nodens ID (`IdMin-IdMax`).

Klassen ska innehålla följande metoder:
* Konstruktorn:
    * Tar emot ett nod-ID.
    * Ska markeras `explicit` och `noexcept`.
* `isValid()`:
    * Returnerar `true` om nod-ID:t är giltigt, annars `false`.
    * Ska markeras `const` och `noexcept`.
* `requestId()`:
    * Returnerar `RequestOffset + myId`.
    * Detta är ID:t som noden ska lyssna på för requests.
    * Ska vara privat.
    * Ska markeras `const noexcept`.
* `responseId()`:
    * Returnerar `ResponseOffset + myId`.
    * Detta är ID:t som noden svarar med.
    * Ska vara privat.
    * Ska markeras `const noexcept`.
* `receive()`:
    * Tar emot en referens till en frame (`const Frame&`).
    * Jämför framens ID med `requestId()`.
    * Skriver ut frame-data vid matchning, gör annars ingenting.
    * Ska markeras `noexcept`.

---

### 5 – Skapa en CAN-buss
I filen `can/bus.h`, implementera en klass döpt `can::Bus`.

Klassen ska innehålla följande privata konstanter (`static constexpr`):
* `MaxNodeCount` = `10U`.

Klassen ska innehålla följande medlemsvariabler:
* `myNodes[MaxNodeCount]`: En array med pekare till noder (`Node*`).
* `myNodeCount`: Antal noder som finns på bussen.

Klassen ska innehålla följande metoder:
* Konstruktorn:
    * Initierar `myNodes` samt `myNodeCount`.
    * Ska markeras `noexcept`.
* `addNode()`:
    * Tar emot en referens till noden som ska läggas till (`Node&`).
    * Lägger till noden om utrymme finns.
    * Returnerar `true` om noden lades till, annars `false`.
    * Ska markeras `noexcept`.
* `send()`:
    * Tar emot en referens till en frame (`const Frame&`).
    * Skriver ut frame-data.
    * Skickar framen till alla noder i `myNodes`.
    * Anropar varje nods `receive()`-metod.
    * Ska markeras `noexcept`.

---

### 6 – Skapa en master  
I filen `can/master.h`, implementera en klass döpt `can::Master`.

Klassen ska innehålla följande medlemsvariabler:
* `myBus`: En referens till en buss (`Bus&`).

Klassen ska innehålla följande metoder:
* Konstruktorn:
    * Tar emot en referens till en buss (`Bus&`).
    * Lagrar referensen i `myBus`.
    * Ska markeras `explicit` och `noexcept`.
* `requestStatus()`:
    * Tar emot ett nod-ID (`std::uint16_t`).
    * Skapar en frame med ID `RequestOffset + nodeId`.
    * Sätter `dlc` till `0U`.
    * Skickar framen via bussen.
    * Ska markeras `noexcept`.

---

### 7 – Testprogram
Använd följande program i `main.cpp` för att testa implementationen:

```cpp
#include <cstdint>

#include "can/bus.h"
#include "can/frame.h"
#include "can/master.h"
#include "can/node.h"

namespace
{
constexpr std::uint16_t CanNodeId1{1U};
constexpr std::uint16_t CanNodeId2{2U};
constexpr std::uint16_t CanNodeId3{3U};
} // namespace

int main()
{
    // Create CAN nodes.
    can::Node node1{CanNodeId1};
    can::Node node2{CanNodeId2};
    can::Node node3{CanNodeId3};

    // Create CAN bus and add nodes.
    can::Bus bus{};
    bus.addNode(node1);
    bus.addNode(node2);
    bus.addNode(node3);

    // Create CAN master and request status from node 3.
    can::Master master{bus};
    master.requestStatus(CanNodeId3);
    return 0;
}
```

Vid körning ska:
* Alla noder ta emot request-framen.
* Endast nod 3 reagera på meddelandet.
* Endast nod 3 skriva ut frame-data.

---
