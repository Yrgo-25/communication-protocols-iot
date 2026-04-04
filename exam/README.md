# Tentamen - Kommunikationsprotokoll & IoT

## Information
Tentamen täcker:
* Design av egna kommunikationsprotokoll (frame‑struktur).
* Skillnaden mellan byte‑ström och strukturerad frame.
* Serialisering och deserialisering.
* Validering (SOF, längd, typ, checksum).
* State‑machine parsing.
* Adressering (DST/SRC) och routing mellan noder.
* Sekvensnummer (SEQ) och dess syfte.
* Skillnaden mellan integritet (checksum) och leveransgaranti (ACK/retry).
* Timeout och retry‑mekanismer.
* Dubbletthantering.
* Översiktlig förståelse för RS‑485, CAN, I2C och MQTT.
* Lagerindelning (Frame → Parser → Transport → Applikation).

---

### Uppdelning
Tentamen är uppdelad i:
* **G‑uppgifter** - Grundläggande förståelse för:
    * Frame‑design.
    * Parserlogik.
    * Adressering och sekvensnummer.
    * Enkel analys av kommunikationsflöden.
* **VG‑uppgifter** - Fördjupning inom:
    * Tillförlitlig kommunikation (ACK/NACK, timeout, retry)
    * Robust felhantering (skriv ned hur systemet återhämtar sig).
    * Analys av dubbletter och fel ordning.
    * Arkitektur och lagerindelning.

---

### Genomförande
Tentamen genomförs med papper och penna:
* Inga hjälpmedel är tillåtna.
* Byte‑sekvenser och checksum‑beräkningar ska kunna göras manuellt.
* Ni ska kunna rita frames och parser‑state‑diagram.

---

## Övningstenta
* Följande [övningstentamen](./practice_exam.md) visar vad som ni förväntas kunna.
* Övningstentamen går att ladda ned som PDF [här](./practice_exam.pdf).

---
