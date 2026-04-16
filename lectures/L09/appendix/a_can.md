# Bilaga A

## Introduktion till CAN (Controller Area Network)

### Översikt
**Controller Area Network (CAN)** är ett kommunikationsprotokoll som ofta används i inbyggda system inom fordonsindustrin.

CAN medför att flera enheter kan kommunicera över en delad buss med ett robust och effektivt meddelandebaserat protokoll.

Typiska CAN‑system innehåller många noder, till exempel:
* Sensorer.
* Styrenheter.
* Aktuatorer.
* Instrumentpaneler.

Alla noder delar samma kommunikationsbuss.

Exempel:

```text
Motorstyrning
        |
Sensor --- CAN‑BUSS --- Instrumentpanel
        |
        ABS
```

Istället för många punkt‑till‑punkt‑anslutningar kan alla enheter kommunicera via en gemensam buss.

---

### Varför CAN används
I många inbyggda system måste flera enheter kommunicera med varandra.

Om man använder punkt‑till‑punkt‑kommunikation (t.ex. UART) krävs många kablar mellan sensorer, instrumentpaneler, aktuatorer med mera. Detta blir särskilt påtagligt när systemet växer.

CAN löser detta genom att erbjuda:
* Delad buss.
* God störimmunitet.
* Multi‑master‑kommunikation.
* Automatisk kollisionshantering.

CAN används därför ofta i:
* Fordonssystem.
* Industriell automation.
* Robotik.
* Inbyggda styrsystem.

---

### CAN‑bussens princip
Ett CAN‑nätverk består av flera noder som är anslutna till samma buss.

```text
     Nod A
       |
     Nod B ---- CAN‑BUSS ---- Nod C
       |
     Nod D
```
Viktiga egenskaper:
* Alla noder kan sända.
* Alla noder tar emot alla meddelanden.
* Varje nod avgör själv om meddelandet är relevant.

Detta kallas broadcast‑kommunikation.

---

### CAN‑ram (CAN Frame)
Ett CAN‑meddelande kallas en CAN frame.

En förenklad CAN‑frame innehåller tre viktiga fält:

```text
+-----------+-----+-----------+
| ID        | DLC | DATA      |
| (11 bit)  |     | 0–8 byte  |
+-----------+-----+-----------+
```

#### ID (Identifier)
Identifieraren beskriver typen av meddelande.

Exempel:

```text
ID      Betydelse
------- -----------------
0x301   Motorvarvtal
0x302   Temperatur
0x400   Statusförfrågan
```

Identifieraren bestämmer även prioritet.

Lägre numeriskt ID innebär högre prioritet.

#### DLC (Data Length Code)
DLC anger hur många byte (0-8) som finns i datapayloaden.

#### DATA
Datafältet innehåller själva informationen som skickas.

Exempel:
* Temperatur.
* Varvtal.
* Sensorstatus.
* Position.

---

### Broadcast‑kommunikation
Till skillnad från exempelvis I2C använder CAN inte traditionella nodadresser:
* Meddelanden identifiera via ett ID som beskriver innehållet, inte mottagaren.
* ID beskriver vad meddelandet innehåller – inte vem som skickar det.

Detta innebär att flera noder kan reagera på samma meddelande:
* Alla noder ser varje frame.
* Varje nod kontrollerar sedan om meddelandet är relevant genom att jämföra framens ID mot sitt request-ID.

Exempel i mjukvara:

``` cpp
if (frame.id == myRequestId)
{
    respond();
}
```

Filtreringen kan göras i mjukvara eller direkt i CAN‑hårdvaran.

---

### Exempel på meddelandefiltrering
Antag att vi designar ett enkelt CAN‑protokoll:

```text
StatusRequest  = 0x300 + nodeId
StatusResponse = 0x400 + nodeId
```

Exempel:

```text
Nod        Request ID   Response ID
---------- ------------ -------------
Sensor 1   0x301        0x401
Sensor 2   0x302        0x402
Sensor 3   0x303        0x403
```

Om mastern skickar:

```text
ID = 0x303
```

kommer endast sensor 3 att svara.

---

### Representation i mjukvara
I programkod kan en CAN‑frame representeras med en enkel struktur, såsom visas nedan:

``` cpp
struct Frame
{
    static constexpr std::size_t MaxDataLen{8U};
    std::uint8_t data[MaxDataLen];
    const std::uint16_t id;
    std::uint8_t dlc;

    explicit Frame(const std::uint16_t id) noexcept
        : data{}
        , id{id}
        , dlc{}
    {}
};
```

Exempel:

``` cpp
Frame frame{0x303};
```

En sensor kan då kontrollera:

``` cpp
if (frame.id == (0x300U + nodeId))
{
    // Svara med sensordata.
}
```

---

### Arbitration (kollisionshantering)
En viktig egenskap hos CAN är icke‑destruktiv arbitration:
* Flera noder kan börja sända samtidigt.
* Bussen avgör automatiskt vilken frame som fortsätter.

Exempel:

```text
Nod A: 01001000001
Nod B: 01000000001
```

CAN använder två signalnivåer:
* Dominant (logisk nolla).
* Recessiv (logisk etta).

Arbitration sker bit för bit medan noder sänder samtidigt:
* Dominanta bitar skriver över recessiva bitar.
* En nod som skickar recessiv bit men läser dominant bit inser att den har förlorat och avbryter.

Resultat:
* Meddelandet med lägst ID vinner.
* Övriga noder avbryter sändningen.

Detta gör att CAN kan undvika kollisioner.

---

### Hårdvarustöd
I verkliga system hanteras många detaljer av CAN‑kontrollern:
* Bittiming
* Arbitration
* CRC
* Felhantering
* Omsändning
* Filtrering

Många mikrokontrollers har inbyggd CAN‑hårdvara, till exempel:
* STM32 CAN
* ESP32 TWAI
* MCP2515

I mjukvara arbetar man oftast med enkla funktioner i stil med följande:

```
send(frame)
receive(frame)
```

---

# Sammanfattning
Viktiga egenskaper hos CAN:
* Delad kommunikationsbuss.
* Broadcast‑baserad kommunikation.
* Meddelanden identifieras via ID.
* Automatisk kollisionshantering.
* Hög robusthet i störiga miljöer.

Därför används CAN i många inbyggda system och fordonssystem.

---
