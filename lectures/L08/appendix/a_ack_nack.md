# Bilaga A

## ACK och NACK
ACK och NACK är två frametyper, som används för att indikera att en frame togs emot (eller inte)
av önskad mottagare:
* ACK (*Acknowledge*) indikerar att mottagaren tog emot framen..
* NACK (*Not Acknowledge*) indikerar att mottagaren inte tog emot framen på grund av något fel (timeout, trasig frame eller liknande).

### Varför räcker inte checksumma?
Checksumma/CRC hjälper oss att upptäcka att data är trasig.

Men checksumman indikerar inte:
* Om en frame tappades.
* Om mottagaren faktiskt tog emot framen.
* Om mottagaren hann processa framen.

Följande gäller:
* Checksumma medför integritet, men inte leveransgaranti.
* För att få detta används normalt ACK- och NACK-frames.
* Om en frame är för trasig kan det vara svårt att skicka ett NACK till sändaren; därmed behövs
även timeouts och retries (behandlas i L06) för att få fullständig leveransgaranti.

---

### ACK och NACK som meddelandetyper
I vårt protokoll inför vi följande meddelandetyper:
* `Type::Ack`
* `Type::Nack`

Regel:
* ACK/NACK refererar alltid till `SEQ` i headern.

Exempel:
* `node1` skickar StatusRequest med `SEQ = 0x0123` till `node2`.
* `node2` tar emot denna request och skickar tillbaka ACK med `SEQ = 0x0123` till `node1`,
för att indikera att framen har tagits emot.

---

#### När skickas ACK?
Policy för kursen (enkel och tydlig):
* Alla applikationsframes (Ping, Pong, StatusRequest, StatusResponse) ska ACKas när de mottagits korrekt och är adresserade till noden.

För en given nod som tar emot en frame gäller att:
* ACK skickas till sändarnoden om framen deserialiserades korrekt.
* ACK skickas endast om noden är den avsedda mottagaren.

---

#### När skickas NACK?
NACK är mer subtil. 

En enkel policy är att:
* Om parsern lyckas extrahera en frame men `deserialize()` misslyckas → försök skicka NACK.

Begränsning:
* Om framen är så trasig att vi inte kan läsa DST/SRC/SEQ på ett säkert sätt kan vi inte routa ett NACK korrekt.
* Då blir det "tyst fel" och sändaren får timeout (behandlas i L06).

Detta är bra pedagogiskt:
* NACK är "nice to have".
* Timeout/retry är det robusta skyddsnätet.

---

### Koppling till felmodell (L04)
Med en deterministisk felmodell kan vi visa:
* Korruption → checksum fail → ingen korrekt leverans → NACK (om möjligt).
* Drop → inget mottaget → ingen ACK/NACK → timeout i L06.
* Fördröjning → ACK kommer sent → timeoutgräns blir viktig i L06.

---

### Transportlogik (för tillfället utan timeout)
I L05 bygger vi ett tunt transportlager som endast:
* Tar emot applikationsframes (från noden).
* Skickar ACK eller NACK.
* Exponerar "utgående frames" som noden kan sända via bussen.

I L06 utökas detta med att:
* "väntan på ACK"
* "timeout"
* "retry"

---
