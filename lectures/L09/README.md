# L09 - CAN (Controller Area Network)

## Dagordning
* Introduktion till CAN.
* Jämförelse: vårt protokoll vs CAN.
* Koppling till verkliga system (ESP32-S3 / TWAI).
* Förberedelse inför CANalyzer-labb.

---

## Mål med lektionen
* Förstå vad CAN är och varför det används.
* Kunna förklara:
  * CAN-ID (identifiering + prioritet).
  * DLC och begränsad payload.
  * Hårdvaruarbitrering.
* Förstå vad hårdvaran hanterar jämfört med vårt protokoll.
* Vara förberedd inför CANalyzer-labben.

---

## Förutsättningar
* Genomförande av L01–L08.

---

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_can.md) för information om CAN.

### Under lektionen
* CAN-frame (översikt):
  * ID
  * DLC
  * DATA
* Arbitrering:
  * Flera noder kan sända samtidigt.
  * Lägst ID vinner (ingen kollision).
* Filtrering:
  * Noder lyssnar på relevanta ID:n.
* Jämförelse:
  * Vårt protokoll: Parsing, routing, tillförlitlighet i mjukvara.
  * CAN: Mycket hanteras i hårdvara.

---

## Utvärdering
* Vad är skillnaden mellan CAN-ID och DST/SRC?
* Vad slipper vi implementera i CAN jämfört med vårt protokoll?

---

## Nästa lektion
* Vector CANalyzer-labb (obligatorisk).

---