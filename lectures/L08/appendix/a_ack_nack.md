# Bilaga A

## ACK och NACK
ACK och NACK är två typer av meddelanden som används för att indikera om en frame har tagits emot korrekt:
* ACK (*Acknowledge*) indikerar att mottagaren har tagit emot framen korrekt.
* NACK (*Negative Acknowledge*) indikerar att något gick fel vid mottagningen.

---

## Varför räcker inte checksumma?
Checksumma används för att upptäcka fel i data (integritet). Men checksumma säger dock inget om:
* En frame tappades på vägen.
* Mottagaren faktiskt tog emot framen.
* Mottagaren hann behandla framen.

Detta innebär att:
* Checksumma ger integritet.
* ACK/NACK ger information om leverans.

För fullständig leveransgaranti krävs även:
* Timeout.
* Omsändning (retry).

---

## ACK/NACK och sekvensnummer (SEQ)
För att koppla ett svar till rätt frame används ett sekvensnummer (SEQ):
* En skickad frame har ett SEQ.
* Mottagaren svarar med ACK eller NACK med samma SEQ.
* Sändaren kan därmed avgöra vilken frame som bekräftas.

Exempel:
* En nod skickar en request med ett visst SEQ.
* Mottagaren svarar med ACK med samma SEQ.
* Sändaren vet då att just den framen nådde fram.

---

## När skickas ACK?
ACK används endast för frames som saknar ett naturligt svar.

ACK skickas när en frame:
* Har tagits emot korrekt.
* Är adresserad till mottagaren.
* Inte ingår i ett request/response-par.

Exempel där ingen ACK/NACK skickas (svaret fungerar som kvittens):
* Request/response:
  * Ping → Pong
  * StatusRequest → StatusResponse  

Exempel där ACK/NACK skickas:
* Envägsmeddelanden (t.ex. kommandon som inte förväntar sig svar)

---

## När skickas NACK?
NACK kan skickas när:
* En frame kan identifieras men innehåller fel.

Begränsning:
* Om framen är för trasig kan viktig information saknas.
* Då går det inte att skicka ett korrekt NACK.

I dessa fall uppstår ett tyst fel:
* Ingen ACK eller NACK skickas.
* Sändaren måste istället använda timeout.

---

## Koppling till felmodell
Olika fel ger olika beteenden:
* Korrupt data → fel upptäcks → NACK (om möjligt).
* Tappad frame → inget svar → timeout.
* Fördröjning → sent svar → timeoutgränser blir viktiga.

---

## Helhetsbild
ACK/NACK är en del av en större lösning:
* Checksumma → upptäcker fel.
* ACK/NACK → signalerar mottagning.
* Timeout → hanterar uteblivna svar.
* Retry → försöker igen.

Tillsammans ger detta ett robust kommunikationssystem.

---
