# L09 - CAN och CANalyzer-labb

## Dagordning
* Introduktion till laborationen och CANalyzer.
* Testkörning av egna CAN-noder.
* Observation och analys av CAN-trafik.
* Felsökning av CAN-kommunikation.

---

## Mål med lektionen
* Kunna:
    * Identifiera en CAN-frame i CANalyzer.
    * Tolka:
        * CAN-ID.
        * DLC.
        * DATA.
    * Verifiera kommunikation mellan två CAN-noder.
* Förstå:
    * Hur flera noder kommunicerar på samma CAN-buss.
    * Hur prioritet (arbitrering) påverkar trafiken.
    * Hur filtrering via CAN-ID fungerar i praktiken.
    * Varför terminering behövs på CAN-bussen.

---

## Förutsättningar
* Grundläggande förståelse för:
    * CAN-frame (ID, DLC, DATA).
    * CAN-ID som identifierare och prioritet.
    * Arbitrering och broadcast.
* Färdigställd eller delvis fungerande CAN-driver för MCP2515.

---

## Instruktioner

### Förberedelse
* Repetera materialet från föregående CAN-föreläsning:
    * CAN-frame (ID, DLC, DATA).
    * Arbitrering.
    * Filtrering via CAN-ID.
    * Terminering.
* Läs igenom [labb-PM:et](../L07/appendix/lab/README.md) om ni inte redan har gjort det.
* Säkerställ att:
    * MCP2515-biblioteket är installerat.
    * CAN-koden kan kompileras.
    * Två Arduino-enheter finns tillgängliga per grupp.

---

## Under lektionen

### Introduktion med representant från Vector
* Genomgång av:
    * CANalyzer.
    * CAN-interfacet.
    * Hur CAN-trafik visas och loggas.
    * Grundläggande felsökning.

---

### Testkörning
* Koppla ihop:
    * Två Arduino-enheter.
    * MCP2515-moduler.
    * CAN-bussen.
* Verifiera:
    * Att CAN-noderna startar korrekt.
    * Att CAN-meddelanden skickas.
    * Att LED-status uppdateras via CAN.

---

### Observation
Studera CAN-trafik i CANalyzer:
* Identifiera:
    * CAN-ID.
    * DLC.
    * DATA.
* Notera:
    * Vilka meddelanden som förekommer ofta.
    * Skillnader mellan olika meddelanden.
    * Eventuell periodicitet.

---

### Analys
* Försök tolka betydelsen av olika meddelanden:
    * Vad representerar olika CAN-ID?
    * Vilka meddelanden verkar viktigast?
* Observera:
    * Prioritet/arbitrering.
    * Skillnader mellan lokala och mottagna händelser.
    * Hur knapptryckningar påverkar trafiken.

---

## Utvärdering
* Hur skiljer sig CAN från vårt tidigare protokoll?
* Vilka problem uppstod vid fysisk CAN-kommunikation?

---

## Nästa lektion
* Övningsuppgifter med ACK/NACK.
* Fortsatt arbete med **P02+**.
