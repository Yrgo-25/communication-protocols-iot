# L11 - Timeout, retries och dubbletthantering (del I)

## Dagordning
* Repetition: ACK/NACK.
* När svar uteblir.
* Timeout.
* Retry (samma SEQ).
* Koppling till dubbletter.
* Fortsatt arbete med **P02+**.

---

## Mål med lektionen
* Förstå varför ACK/NACK inte räcker utan timeout.
* Förstå hur timeout används för att upptäcka uteblivna svar.
* Förstå hur retry fungerar (samma SEQ).
* Förstå hur retries leder till dubbletter.
* Förstå behovet av dubbletthantering.

---

## Förutsättningar
* Genomförande av L01–L10.

---

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_timeout_retry_duplicate.md) för information om timeouts, retries samt dubbletthantering.

### Under lektionen
### Under lektionen
* Genomgång av timeout och retry.
* Övningen genomförs i helklass:
  * Analysera vad som händer när svar uteblir.
  * Följ flödet: skickad frame → timeout → retry.
  * Identifiera hur dubbletter uppstår.
* Fortsätt med **P02+** i grupper.
* Övningsuppgifterna i [bilaga B](./appendix/b_exercises.md) är valfria och kan användas som stöd eller referens.

---

## Utvärdering
* Vad är skillnaden mellan NACK och utebliven ACK?
* Vad händer när timeout inträffar?
* Varför används samma SEQ vid retry?
* Hur uppstår dubbletter i samband med retry?

---

## Nästa lektion
* Fortsättning på dagens tema.
* Fortsatt arbete med **P02+**.

---