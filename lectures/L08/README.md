# L08 - ACK och NACK (del I)

## Dagordning
* Repetition: Fel i kommunikationen.
* Varför räcker inte checksumma?
* Kvittens (ACK och NACK).
* Koppling till protokollet (SEQ, TYPE).
* Fortsatt arbete med **P02+**.

---

## Mål med lektionen
* Förstå skillnaden mellan integritet (checksumma) och leveransgaranti (ACK).
* Förstå hur ACK/NACK används i protokollet.
* Förstå kopplingen mellan SEQ och ACK.
* Förstå varför frånvaro av ACK leder till timeout.

---

## Förutsättningar
* Genomförande av L01–L06.

---

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_ack_nack.md) för information om ACK och NACK.

### Under lektionen
* Genomgång av ACK/NACK.
* Gemensam övning:
  * Identifiera behov av ACK.
  * Koppla ACK till SEQ.
  * Analysera vad som händer vid utebliven ACK.
* Fortsätt med **P02+** i grupper.
* Övningsuppgifterna i [bilaga B](./appendix/b_exercises.md) är valfria och kan användas som stöd eller referens.

---

## Utvärdering
* Varför räcker inte checksumma för leveransgaranti?
* Vad är syftet med att inkludera SEQ i både request och ACK?
* Vad händer om ACK uteblir?

---

## Nästa lektion
* Fortsättning på dagens tema.
* Fortsatt arbete med **P02+**.

---
