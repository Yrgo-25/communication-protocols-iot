# L04 - Databussar och routing (del II)

## Dagordning
* Repetition: Buss och noder.
* Gemensam övning: Status response till status request mellan noder:
  * Serialisering av frame (frame → bytes).
  * Sändning via buss (byte för byte, FIFO).
  * Parsning av bytes på nodnivå.
  * Extrahering av frame (bytes → frame).
  * Filtrering på destinationsadress.
  * Hantering av frame vid korrekt destination.
* Fortsatt arbete med **P02+**.

---

## Mål med lektionen
* Förstå hela flödet från sändning till mottagning (frame → buss → parser → frame).
* Kunna ta emot och tolka frames i en nod.
* Kunna filtrera på destinationsadress.
* Kunna svara på en förfrågan (request → response).

---

## Förutsättningar
* Genomgång av L01–L03.

---

## Instruktioner

### Under lektionen
* Övningen genomförs i helklass.
* Fortsätt med **P02+** i grupper.
* Övningsuppgifterna från [bilaga B](../L03/appendix/b_exercises.md) (L03) är valfria och kan användas som stöd eller referens.

---

## Utvärdering
* Beskriv hela flödet från sändning till mottagning:
  * frame → buss → nod → parser → frame.
* Förstå varför en komplett frame krävs innan data används.
* Förstå filtrering baserat på destinationsadress (DST).

---

## Nästa lektion
* Felmodell för databussar.
* Fortsatt arbete med **P02+**.

---
