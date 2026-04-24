# L10 - Vector CANalyzer-labb

## Dagordning
* Introduktion till labben och verktyget (CANalyzer).
* Observation av CAN-trafik.
* Analys och tolkning av CAN-meddelanden.
* Koppling mellan teori (L09) och verklig trafik.

---

## Mål med lektionen
* Kunna:
    * Identifiera en CAN-frame i CANalyzer.
    * Tolka:
        * CAN-ID.
        * DLC.
        * DATA.
* Förstå:
    * Hur flera noder kommunicerar på en CAN-buss.
    * Hur prioritet (arbitrering) påverkar trafiken.
    * Hur filtrering på CAN-ID fungerar i praktiken.
* Kunna koppla:
    * Teorin från L09 till verklig CAN-kommunikation.
    * CAN till det protokoll ni implementerade tidigare i kursen.

---

## Förutsättningar
* Förståelse för:
    * CAN-frame (ID, DLC, DATA).
    * CAN-ID som identifierare och prioritet.
    * Grundläggande koncept kring arbitration och broadcast.
* Genomgången L09 (CAN).

---

## Instruktioner

### Förberedelse
* Repetera materialet från L09:
    * CAN-frame (ID, DLC, DATA).
    * Arbitration.
    * Filtrering via CAN-ID.
* Ingen ytterligare förberedelse krävs.

---

## Under lektionen

### Introduktion (kort)
* Genomgång av CANalyzer:
    * Hur CAN-trafik visas.
    * Hur man identifierar en frame.
    * Grundläggande navigation i verktyget.

---

### Observation
Studera CAN-trafik i verktyget:
* Identifiera:
    * CAN-ID
    * DLC
    * DATA
* Notera:
    * Vilka meddelanden som förekommer ofta.
    * Skillnader mellan olika meddelanden.

---

### Analys
* Försök tolka betydelsen av meddelanden:
    * Vad kan olika ID representera?
    * Hur skiljer sig olika typer av meddelanden?
* Observera:
    * Prioritet (vilka meddelanden dominerar bussen?).
    * Eventuell periodicitet.

---

### Koppling till kursen
* Jämför med vårt protokoll:
    * Var finns:
        * Adressering?
        * Integritet (CRC)?
        * Parser?
* Vad slipper vi implementera i CAN jämfört med vårt system?
* Vad finns fortfarande kvar på applikationsnivå?

---

## Mål
* Kunna peka ut och tolka en CAN-frame i CANalyzer.
* Kunna förklara vad ett CAN-ID representerar.
* Kunna beskriva hur CAN skiljer sig från vårt protokoll.

---

## Utvärdering
* Kände ni igen strukturen från L09 i verklig trafik?
* Vad var mest oväntat i CANalyzer?
* Förstår ni hur CAN förenklar jämfört med vårt protokoll?

---

## Nästa lektion
* Timeout och retries.
* Fortsatt arbete med **P02+**.

---
