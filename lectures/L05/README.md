# L05 - Felmodell för databussar (del I)

## Dagordning
* Repetition: Fungerande kommunikation (L04).
* När det går fel:
  * Tappade bytes.
  * Korrupt data.
  * Fördröjning.
* Felmodell i buss.
* Fortsatt arbete med **P02+**.

---

## Mål med lektionen
* Förstå att kommunikation inte är perfekt.
* Förstå hur olika fel uppstår i en databuss:
  * Tappade bytes.
  * Korrupt data.
  * Fördröjning.
* Förstå hur fel påverkar byte-ström och frames.
* Förstå varför tillförlitlighet behövs (ACK/NACK).

---

## Förutsättningar
* Genomförande av L01–L04 (kommunikation fungerar).

---

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_error_model.md) för information gällande felmodeller.

### Under lektionen
* Genomgång av felmodeller.
* Fortsätt med **P02+** i grupper.
* Övningsuppgifterna i [bilaga B](./appendix/b_exercises.md) är valfria och kan användas som stöd eller referens.

---

## Utvärdering
* Beskriv hur följande fel påverkar kommunikationen:
  * Tappade bytes.
  * Korrupt data.
  * Fördröjning.
* Hur påverkas frame-parsern av fel i byte-strömmen?
* Varför felaktiga frames måste ignoreras.
* Förstå begränsningar i en lösning utan tillförlitlighet (ACK/NACK).
* Beskriv vad som krävs för att upptäcka och hantera fel.

---

## Nästa lektion
* Fortsättning på dagens tema.
* Fortsatt arbete med **P02+**.

---