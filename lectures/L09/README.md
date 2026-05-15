# L09 - CAN (Controller Area Network)

## Dagordning
* Introduktion till CAN.
* Jämförelse av eget kommunikationsprotokoll vs CAN.
* Förberedelser inför CAN- och CANalyzer-labben.
* Fysisk CAN-buss och terminering.

---

## Mål med lektionen
* Förstå vad CAN är och varför det används inom inbyggda system.
* Kunna förklara:
  * CAN-ID och prioritering.
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
* Inför laborationen:
  * Läs igenom bifogat [labb-PM](./appendix/lab/README.md).
  * Implementera driver för CAN-kontrollern `MCP2515`.
  * Koppla ihop två Arduino-enheter med `MCP2515` CAN-moduler.
  * Installera och konfigurera Vector CANalyzer (en dator per grupp).

---

## Utvärdering
* Vad är skillnaden mellan CAN-ID och DST/SRC?
* Vad slipper vi implementera i CAN jämfört med vårt protokoll?
* Varför behövs arbitrering i CAN?
* Varför används terminering på CAN-bussen?

---

## Nästa lektion
* Vector CANalyzer-labb (obligatorisk).

---