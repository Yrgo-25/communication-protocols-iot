# L12 - Timeout, retries och dubbletthantering (del II)

## Dagordning
* Repetition: Timeout och retry (L11).
* Dubbletter i praktiken:
  * Förlorad ACK → retry → samma frame igen.
* Korrekt hantering:
  * Identifiera med SEQ.
  * Ignorera dubbletter.
  * Skicka ACK igen.
* **P02+**:
  * Säkerställa korrekt leverans.

---

## Mål med lektionen
* Kunna identifiera dubbletter med hjälp av SEQ.
* Kunna undvika att samma frame behandlas flera gånger.
* Kunna skicka ACK även för dubbletter.
* Förstå vad som krävs för robust leverans.

---

## Förutsättningar
* Genomförande av L01–L11.

---

## Instruktioner

### Under lektionen
* Fortsätt med övningsuppgifterna i [bilaga B](../L11/appendix/b_exercises.md) från L11.

---

## Utvärdering
* Vad händer om samma frame behandlas två gånger?
* Hur identifieras dubbletter?
* Varför ska ACK skickas även för dubbletter?

---

## Nästa lektion
* Repetition och förberedelse inför dugga.
* Fortsatt arbete med **P02+**.

---
