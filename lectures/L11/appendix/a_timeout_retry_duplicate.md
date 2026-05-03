# Bilaga A

## Timeout, retries och dubbletthantering

### Timeout
I inbyggda system används timeout för att upptäcka att ett svar uteblir.

Ett typiskt förlopp:
**1.** En nod skickar en frame som förväntar sig svar (t.ex. ACK eller respons).  
**2.** Noden startar en intern timer.  
**3.** Noden väntar på svar med matchande SEQ.  
**4.** Om inget svar kommer inom tidsgränsen → timeout inträffar.  

Timeout innebär att:
* Kommunikation antas ha misslyckats.
* Systemet måste vidta åtgärd (t.ex. retry).

---

### Retry
Retry innebär att samma frame skickas igen efter timeout:
* Samma SEQ används.
* Samma payload skickas.
* Antalet försök räknas (retry-counter).
* Ett max antal retries definieras (t.ex. 3).

Om max antal retries uppnås:
* Felet rapporteras vidare.
* Vidare försök stoppas.

---

### Dubbletthantering
Om ett svar (t.ex. ACK) tappas:
* Sändaren gör en retry.
* Samma frame skickas igen.
* Mottagaren får samma frame flera gånger.

Utan dubbletthantering:
* Samma logik kan köras flera gånger.
* Systemet kan ge fel resultat.

Exempel:
* Ett kommando utförs flera gånger.
* Ett värde uppdateras flera gånger.

För att lösa detta måste mottagaren kunna känna igen dubbletter. Detta görs genom att:
* Identifiera frames med samma SEQ.
* Avgöra om framen redan har behandlats.

Vid dubblett:
* Applikationslogik körs inte igen.
* Samma svar (t.ex. ACK eller respons) kan skickas igen.

---

## Helhetsbild
Timeout, retry och dubbletthantering hänger ihop:
* Timeout → upptäcker uteblivet svar.
* Retry → försöker igen.
* Dubbletthantering → förhindrar fel vid upprepade frames.

Tillsammans gör detta kommunikationen robust.

---
