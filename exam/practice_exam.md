# Övningsdugga – Kommunikation & IoT

## Information

### Hjälpmedel
* Papper och penna.

### Poänggränser och betygsnivåer
Totalt: 25 poäng.

Betygsgränser:
* **G:** Minst 12 poäng.
* **VG:** Minst 19 poäng.

Bidrag till kursens slutpoäng:
* Betyget **G** ger 2 poäng till kurssammanställningen.
* Betyget **VG** ger 4 poäng till kurssammanställningen.

### Instruktioner
* Svara kort och tydligt.
* Anta att vår checksumma är 16-bitars summering av alla bytes fram till CHK-fältet.
* Fält större än 1 byte (SOF, SEQ, CHK) skickas big-endian.

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

<div style="page-break-before: always;"></div>

### **3.** Parser (3p)
Anta att en frame-parser använder följande tillstånd:
* `WaitForSof1`
* `WaitForSof2`
* `WaitForLen`
* `WaitForType`
* `WaitForDst`
* `WaitForSrc`
* `WaitForSeq1`
* `WaitForSeq2`
* `WaitForPayload`
* `WaitForChk1`
* `WaitForChk2`
* `Ready`

**a)** Beskriv kort vad parsern gör i `WaitForSof1` och `WaitForSof2`.  
**b)** Vad är en rimlig strategi om en checksumman blir fel?
**c)** Vad betyder det att parsern ska "återhämta sig" från skräpdata?

---

### **4.** Broadcast-buss och routing (3p)
Vi har en broadcast-buss där alla noder tar emot alla bytes.

**a)** Var sker routing: I bussen eller i noden? Motivera.  
**b)** Varför behövs både `DST` och `SRC`? Ge ett exempel.  
**c)** Nod B får en korrekt frame men `DST != B:s adress`. Vad ska B göra?

---

<div style="page-break-before: always;"></div>

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

### **7.** Fel i byte-ström (3p)
En nod tar emot följande byte-ström:

```text
00 FF A5 F7 03 00 25 17 7F 05 10 20 30 65 99
```

Anta att:
* `SOF = 0xA5F7`
* `LEN = 3`
* Övriga fält följer standardstrukturen.

**a)** Var börjar en frame i strömmen? (ange index eller byteposition)  
**b)** Vilka bytes tillhör framen (från SOF till CHK)?  
**c)** Vad händer med datan innan SOF?  

---

<div style="page-break-before: always;"></div>

### **8.** Felmodell och konsekvenser (6p)
Anta att följande frame skickas:

```text
A5 F7 03 03 25 17 7F 05 10 20 30 68
```

**a)** Anta att byten `0x20` tappas under överföring:

```text
A5 F7 03 03 25 17 7F 05 10 30 68
```

Svara på följande:
* Vad händer med tolkningen av framen?
* Kan mottagaren lita på datan?

**b)** Anta att `0x30` ändras till `0x31`:

```text
A5 F7 03 03 25 17 7F 05 10 20 31 68
```

Svara på följande:
* Ser framen korrekt ut strukturellt?
* Hur kan mottagaren upptäcka felet?

**c)** Anta att data anländer uppdelat över tid:

```text
Tick 1: A5 F7 03
Tick 2: 00 25
Tick 3: 17 7F 05 10
Tick 4: 20 30 65
```

* När kan framen tolkas som komplett?
* Vad händer om systemet försöker tolka framen innan alla bytes har tagits emot?

---
