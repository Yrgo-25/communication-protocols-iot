# Bilaga B

## Övningsuppgifter

Anta att vi använder följande framestruktur:

```
Fält   Storlek   Beskrivning
-----* --------* -------------------------------------------
SOF    2 byte    0xA5F7
LEN    1 byte    Payload-längd
TYPE   1 byte    Meddelandetyp
DST    1 byte    Destination address
SRC    1 byte    Source address
SEQ    2 byte    Sekvensnummer
DATA   N byte    Payload
CHK    2 byte    Checksumma (summa av alla tidigare bytes)
```

Checksumma = summera alla bytes från SOF till sista payloadbyte (`uint16_t`).

Vi har följande frametyper:
* Ping = `0x00`
* Pong = `0x01`
* StatusRequest = `0x02`
* StatusResponse = `0x03`

---

### Del I - Skapande av frames för hand
Anta att en enhet på adress `0x17` ska skicka en statusrequest till en sensor på adress `0x25`. Anta att sekvensnumret är `0x7F05`. Därmed har vi följande data:
* TYPE = 0x02
* DST = 0x25
* SRC = 0x17
* SEQ = 0x7F05
* DATA = tom (0 bytes)

**a)** Rita upp motsvarande StatusRequest-frame på papper:
* Fyll i:
  * SOF
  * LEN
  * TYPE
  * DST
  * SRC
  * SEQ
  * CHK
* Beräkna checksumman manuellt.

**b)** Anta att sensorn svarar med ett 16-bitars sensorvärde `0x3201`, som läggs i payloaden.
Rita upp motsvarande StatusResponse-frame på papper:
* Regler:
  * TYPE ändras till StatusResponse.
  * DST och SRC byter plats.
  * Samma SEQ.
  * DATA = `0x3201` (2 bytes).
Beräkna checksumman manuellt.

---

### Del II - Implementering av frames i C++
Du ska implementera frames i C++ via en strukt `comm::frame::Frame`.  
Er implementation ska valideras via ett befintligt testprogram.

#### **1.** Inspektera filstruktur
Observera katalogen [code](./code/):
* [main.cpp](./code/source/main.cpp) innehåller testprogrammet som ska validera er frame-implementation.
* [comm/frame/type.h](./code/include/comm/frame/type.h) ska innehålla definitioner av frametyper via en enumerationsklass `comm::frame::Type`.
* [comm/frame/frame.h](./code/include/comm/frame/frame.h) ska innehålla strukten `comm::frame::Frame`.
* [comm/frame/frame.cpp](./code/source/comm/frame/frame.cpp) ska innehålla implementationsdetaljer för strukten `comm::frame::Frame`, exempelvis metoddeklarationer.

---

#### **2.** Skapa frame-typer
I [comm/frame/type.h](./code/include/comm/frame/type.h), implementera enumerationsklassen `com::frame::Type` enligt beskrivningen ovan.

---

#### **3.** Skapa frame-strukt
I [comm/frame/frame.h](./code/include/comm/frame/frame.h), implementera strukten `comm::frame::Frame` enligt befintlig dokumentation:
* Lägg till medlemsvariabler enligt beskrivningen:
    * `payload[MaxPayloadLen]`
    * `seqNr`
    * `dstAddr`
    * `srcAddr`
    * `payloadLen`
    * `type`
* Deklarera dokumenterade metoder:
    * `serialize()` ska markeras `const` samt `noexcept`.
    * `deserialize()` ska markeras `noexcept`.

---

#### **4.** Implementera serialiseringsmetod
I [comm/frame/frame.cpp](./code/source/comm/frame/frame.cpp), implementera metoden 
`comm::frame::Frame::serialize()`, så att denna skriver följande till given buffer:
* SOF = `0xA5F7` (big endian).
* LEN: Payload-längden.
* TYPE: Frame-typen.
* DST: Destinationsadressen.
* SRC: Avsändaradressen.
* SEQ: Sekvensnumret (big endian).
* DATA: Payload (i samma ordning som det är lagrat i framens payload-buffer).
* CHK: Checksumma beräknad över samtliga föregående fält, byte för byte.

Returnera:
* Total framelängd om serialiseringen lyckades.
* 0 vid fel.

---

#### **5.** Implementera deserialiseringsmetod
I [comm/frame/frame.cpp](./code/source/comm/frame/frame.cpp), implementera metoden `comm::frame::Frame::deserialize()`, så att denna:
* Validerar given data:
    * SOF är korrekt.
    * LEN inom gräns.
    * Given buffer är tillräckligt stor (måste minst kunna rymma headern).
    * Frametypen är giltigt.
    * Checksumman korrekt.
* Given data ska sparas i framen om all data är giltig.

---

#### **6.** Validera implementationen
Kompilera och kör testprogrammet i [main.cpp](./code/source/main.cpp) i en Linuxterminal:

```bash
make
```

Det som sker i testet är att:
* En ping-frame skapas och skickas från address `0x01` till `0x02`.
* Om ping-framen mottas korrekt skickas en pong-frame tillbaka.
* Respektive frame skrivs ut i terminalen.

Notera:
* I testet antas att er frame-strukt använder de namn för fälten som specificerades ovan, exempelvis `payloadLen` och `dstAddr`.
* Har ni namngett medlemsvariablerna annorlunda är det fritt fram att ändra motsvarande i testet.

---
