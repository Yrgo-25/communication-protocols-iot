# Dugga - Kommunikationsprotokoll & IoT

## Information
Dugga täcker:

* Design av egna kommunikationsprotokoll (frame-struktur).
* Skillnaden mellan byte-ström och strukturerad frame.
* Serialisering och deserialisering.
* Validering (SOF, längd, typ, checksum).
* State-machine parsing.
* Adressering (DST/SRC) och routing mellan noder.
* Sekvensnummer (SEQ) och dess syfte.
* Skillnaden mellan integritet (checksum) och leveransgaranti.
* ACK/NACK och när de används.
* Timeout och retry-mekanismer.
* Dubbletter och deras konsekvenser.
* Fel i byte-ström:
  * Tappade bytes.
  * Korrupt data.
  * Fördröjning.

---

### Uppdelning
Tentamen är uppdelad i:
* **G-uppgifter** - Grundläggande förståelse för:
  * Frame-design och checksum-beräkning.
  * Parserlogik och byte-ström.
  * Adressering (DST/SRC) och sekvensnummer (SEQ).
  * Tolkning av frames i en byte-ström.
* **VG-uppgifter** - Fördjupning inom:
  * Tillförlitlig kommunikation (ACK/NACK, timeout, retry).
  * Analys av fel i byte-ström och deras konsekvenser.
  * Dubbletter och varför de uppstår.
  * Systemets beteende vid uteblivna svar.

---

### Genomförande
Tentamen genomförs med papper och penna:
* Inga hjälpmedel är tillåtna.
* Byte-sekvenser och checksum-beräkningar ska kunna göras manuellt.
* Ni ska kunna tolka och analysera byte-strömmar.

---

## Övningsdugga
* Följande [övningsdugga](./practice_exam.md) visar vad ni förväntas kunna.
* Övningsduggan går att ladda ned som PDF [här](./practice_exam.pdf).

---
