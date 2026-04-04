# L11 - Timeout, retries och dubbletthantering (del I)

## Dagordning
* Repetition: ACK/NACK.
* När ACK uteblir.
* Timeout.
* Retry (samma SEQ).
* **P01**:
  * Införa timeout och retry.

---

## Mål med lektionen
* Förstå varför ACK inte räcker utan timeout.
* Kunna införa timeout i ett tick-drivet system.
* Kunna skicka om frames (retry).
* Förstå kopplingen till dubbletter.

---

## Förutsättningar
* Genomförande av L01–L10.

---

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_timeout_retry_duplicate.md) för information om timeouts, retries samt dubbletthantering.

### Under lektionen
* Genomför övningsuppgifter i [bilaga B](./appendix/b_exercises.md).

---

## Utvärdering
* Vad är skillnaden mellan NACK och utebliven ACK?
* Vad händer när timeout inträffar?
* Varför måste SEQ vara samma vid retry?

---

## Nästa lektion
* Fortsättning på dagens tema.
* Fortsatt arbete med **P01**.

---