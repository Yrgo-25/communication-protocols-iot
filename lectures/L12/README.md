# L12 - Timeout, retries och dubbletthantering (del II)

## Dagordning
* Repetition: Timeout och retry (L11).
* Dubbletter i praktiken:
  * Förlorad ACK → retry → samma frame igen.
* Korrekt hantering:
  * Identifiering av dubbletter.
  * Konsekvenser av upprepade frames.
* Fortsatt arbete med **P02+**.

---

## Mål med lektionen
* Förstå hur dubbletter uppstår i samband med retry.
* Förstå hur dubbletter kan identifieras (t.ex. med SEQ).
* Förstå konsekvenser av att behandla samma frame flera gånger.
* Förstå principer för att undvika fel vid dubbletter.
* Förstå vad som krävs för robust leverans.

---

## Förutsättningar
* Genomförande av L01–L11.

---

## Instruktioner

### Under lektionen
* Genomgång av dubbletthantering.
* Övningen genomförs i helklass:
  * Analysera hur dubbletter uppstår (retry).
  * Identifiera dubbletter i exempel.
  * Diskutera konsekvenser av att behandla samma frame flera gånger.
* Fortsätt med **P02+** i grupper.
* Övningsuppgifterna i [bilaga B](../L11/appendix/b_exercises.md) är valfria och kan användas som stöd eller referens.

---

## Utvärdering
* Vad händer om samma frame behandlas flera gånger?
* Hur kan dubbletter identifieras?
* Varför ska svar (t.ex. ACK eller respons) skickas även för dubbletter?

---

## Nästa lektion
* Repetition och förberedelse inför dugga.
* Fortsatt arbete med **P02+**.

---
