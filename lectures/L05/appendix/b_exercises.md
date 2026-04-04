# Bilaga B

## Övningsuppgifter

### 1. Implementera deterministisk byte-drop
Utöka `comm::bus::Stub`:
* Lägg till byteräknaren `std::uint8_t myByteCounter`.
* Lägg till fördröjningskön `std::uint8_t myDelayCounter`.
* Inkrementera för varje skickad byte.
* Droppa byte nummer 5.

Verifiera att:
* Parsern misslyckas att parsa framen.
* Systemet återgår till vänteläge, dvs. parsern återgår till tillståndet `WaitForSof1`.

---

### 2. Implementera bitflip
Modifiera bussen:

* Om `3U == myByteCounter`:
    * Flippa lägsta biten.

Verifiera att:
* Felet detekteras genom att checksumman blir fel.
* Framen avvisas.

---

### 3. Implementera fördröjningskö
Istället för att leverera byten direkt i `tick()`:
* Lagra byten tillsammans med fördröjningskön `myDelayCounter`, som initieras till `5U`.
* Dekrementera `myDelayCounter` varje tick.
* Leverera först när `myDelayCounter` har räknat ned till `0U`.

Verifiera att:
* Systemet fungerar även med fördröjning.
* Ingen blockering uppstår.

---

### 4. Två frames back-to-back med fel
Skicka:
* PING
* PING

Inför ett fel i den första framen. Verifiera att:
* Den första framen avvisas.
* Den andra framen tas emot korrekt.

---

## Reflektion inför L05
Diskutera:
* Hur vet sändaren att en frame tappades?
* Hur vet mottagaren att sändaren behöver skicka igen?
* Räcker checksumma för tillförlitlighet?

Detta leder naturligt till ACK/NACK i nästa lektion.

---
