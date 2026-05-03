# L08 - ACK och NACK (del II)

## Dagordning
* Repetition: ACK/NACK (L07).
* När kvittens inte räcker:
  * Förlorad ACK.
  * Förlorad respons.
  * Dubbletter (problem).
* Gemensam övning:
  * Analys av fel och konsekvenser.
* Fortsatt arbete med **P02+**.

---

## Mål med lektionen
* Genomgång av begränsningar med ACK/NACK.
* Förstå hur förlorad ACK och förlorad respons påverkar systemet.
* Förstå hur dubbletter kan uppstå.
* Förstå konsekvenser av att behandla samma frame flera gånger.
* Förstå behovet av ytterligare mekanismer (timeout, retry).

---

## Förutsättningar
* Genomförande av L01–L07.

---

## Instruktioner

### Under lektionen
* Genomgång av dubbletter och förlorad ACK.
* Övningen genomförs i helklass:
  * Identifiera dubbletter i byte-strömmen.
  * Analysera konsekvenser av att behandla samma frame flera gånger.
* Fortsätt med **P02+** i grupper.
* Övningsuppgifterna i [bilaga B](../L07/appendix/b_exercises.md) (L07) är valfria och kan användas som stöd eller referens.

---

## Utvärdering
* Vad händer om ett ACK tappas?
* Vad händer om ett svar (t.ex. StatusResponse) tappas?
* Hur kan dubbletter uppstå i systemet?
* Varför räcker inte ACK/NACK för att lösa detta?

---

## Nästa lektion
* Introduktion till CAN (Controller Area Network).
* Förberedelse inför CAN-labben.

---