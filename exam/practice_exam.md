# Övningsdugga – Kommunikation & IoT

## Information

### Hjälpmedel
* Papper och penna.

### Poänggränser och betygsnivåer
Totalt: 22 poäng.

Betygsgränser:
* **G:** Minst 10 poäng.
* **VG:** Minst 16 poäng.

Bidrag till kursens slutpoäng:
* Betyget **G** ger 2 poäng till kurssammanställningen.
* Betyget **VG** ger 4 poäng till kurssammanställningen.

### Instruktioner
* Svara kort och tydligt.
* Anta att vår checksumma är **16-bitars summering av bytes** (modulo `2^16`):
  * `CHK = SUM(all bytes från SOF till sista payloadbyte)`
  * CHK-fältet ingår inte i summeringen.
* Fält större än 1 byte (SOF, SEQ, CHK) skickas **big-endian**.

Vi använder framestrukturen:

```
SOF (2) | LEN (1) | TYPE (1) | DST (1) | SRC (1) | SEQ (2) | DATA (N) | CHK (2)
```

Frametyper:
* Ping = `0x00`
* Pong = `0x01`
* StatusRequest = `0x02`
* StatusResponse = `0x03`
* Ack = `0x04`
* Nack = `0x05`

---

<div style="page-break-before: always;"></div>

## Uppgifter

### **1.** Bygg en frame för hand (StatusRequest) (2p)
En nod A har adress `0x17` och ska skicka en `StatusRequest` till sensor S med adress `0x25`.
Sekvensnumret är `0x7F05`.

**a)** Skriv upp hela framen byte för byte i hex (utan checksumma).
**b)** Beräkna checksumman (visa summeringen) och lägg till denna i framen.

Tips:
* `LEN = 0` (ingen payload)
* `SOF = 0xA5F7`

---

### **2.** Bygg en frame för hand (StatusRequest) (2p)
Sensorn S svarar med sensorvärdet `0x3201` som payload (2 byte, big-endian).

Regler:
* `TYPE = StatusResponse`
* `DST` och `SRC` byter plats
* Samma `SEQ`
* `LEN = 2`
* `DATA = 0x32 0x01`

**a)** Skriv upp hela framen byte för byte i hex (utan checksumma).
**b)** Beräkna checksumman (visa summeringen) och lägg till denna i framen.

---

### **3.** Parser (state machine) (3p)
Anta att parsern använder följande states:

* `WaitForSof1`
* `WaitForSof2`
* `WaitForLen`
* `WaitForHeader`
* `WaitForPayload`
* `WaitForChecksum`
* `Ready`

**a)** Beskriv kort vad parsern gör i `WaitForSof1` och `WaitForSof2`.  
**b)** Vad är en rimlig strategi om en checksumma blir fel (CHK fail)?  
**c)** Vad betyder det att parsern ska "återhämta sig" från skräpdata?

---

<div style="page-break-before: always;"></div>

### **4.** Broadcast-buss och routing (3p)
Vi har en broadcast-buss där alla noder tar emot alla bytes.

**a)** Var sker routing: I bussen eller i noden? Motivera.  
**b)** Varför behövs både `DST` och `SRC`? Ge ett exempel.  
**c)** Nod B får en korrekt frame men `DST != B:s adress`. Vad ska B göra?

---

### **5.** ACK/NACK (3 p)
**a)** När ska en nod skicka `Ack` i vår modell?  
**b)** När kan `Nack` vara svårt eller omöjligt att skicka?  
**c)** Varför måste `Ack/Nack` referera till `SEQ`?

---

### **6.** Timeout och retry (3p)
Node A skickar en applikationsframe som kräver ACK.

**a)** Vad händer om ACK uteblir? Beskriv flödet (timeout → retry).  
**b)** Vid retry: Ska `SEQ` ändras eller vara samma? Varför?  
**c)** Vad är en dubblett och hur ska mottagaren bete sig?

---

<div style="page-break-before: always;"></div>

### **7.** Kodförståelse (3p)
Anta att vi nedanstående har implementerats för en nod:

```cpp
if (myParser.isFrameReady())
{
    Frame rxFrame{};
    myParser.extractFrame(rxFrame);
    if (rxFrame.dst != myAddr) { return; }

    transport.onRxFrame(rxFrame);
    Frame txFrame{};

    if (handleFrame(rxFrame, txFrame))
    {
        constexpr std::size_t txBufLen{64U};
        std::uint8_t txBuf[txBufLen]{};
        const std::size_t serializedBytes{txFrame.serialize(txBuf, txBufLen)};

        for (std::size_t i{}; i < serializedBytes; ++i)
        {
            myBus.sendByte(txBuf[i]);
        }
    }
}
```

**a)** Varför kontrolleras `rxFrame.dst != myAddr` innan `handleFrame()`?  
**b)** Vad är transportens roll här (en mening)?  
**c)** Vad kan hända om man tar bort DST-kontrollen helt i ett broadcast-system?

---

### **8.** Begrepp (3p)
Förklara kortfattat vad följande begrepp innefattar när det kommer till frames:
**a)** Integritet.  
**b)** Leveransgaranti.  
**c)** Timingmodell (tick-driven).

---