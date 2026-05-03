# Bilaga A

## Databuss och routing i inbyggda system
I embedded-system är det vanligt att flera noder delar samma fysiska kommunikationsmedium:
* En nod utgör av en enhet, exempelvis en sensor.
* Kommunikationsmediumet består vanligtvis av en delad buss, där varje nod har en unik adress.

I en sådan topologi är det normalt att:
* Alla noder "ser" samma byte-stream.
* Varje nod måste själv avgöra om ett meddelande är relevant.

---

### Broadcast
I en broadcast-buss skickas varje byte till alla noder i systemet.
Detta innebär att:
* En nod sänder en byte.
* Övriga noder tar emot samma byte.

Routing sker därför inte i bussen, utan i noden.

---

### Routing med DST/SRC
Routing definieras av följande princip:
* `DST` anger destinationsadressen.
* `SRC` anger avsändaradressen.

När en nod extraherar en frame:
* Om destinationsadressen är nodens adress → hantera framen.
* Om destinationsadressen inte är nodens adress → ignorera framen.

**Notera:**
* Bussen är "dum" (den fattar inga beslut).
* Noden är "smart" (den filtrerar och svarar).

---

### Hur knyter detta an till vårt protokoll?
I protokollet har vi:
* `DST` och `SRC` i headern.
* `TYPE` för att tolka budskapet (PING, PONG, STATUS osv.).
* `SEQ` för att kunna koppla ihop request/response (och senare retry/ACK).

Vi ska därmed implementera ett system där:
* Bussen skickar bytes till alla.
* Varje nod kör sin parser.
* När en frame blir klar filtrerar noden på DST och kan svara.

Vi ska därmed implementera stubb-hårdvara i systemet:
* Klassen `comm::node::Stub` ska representera simulerade noder.
* Klassen `comm::bus::Stub` ska representera en simulerad databuss.

För att senare kunna ersätta stubbarna med riktig hårdvara ska interface användas.
* Klassen `comm::node::Interface` ska utgöra ett gemensamt interface för noder.
* Klassen `comm::bus::Interface` ska utgöra ett gemensamt interface för en databuss.

---

### Klock-driven simulering
För att kunna testa och simulera robust ska vi undvika blockering.
Vi gör därför modellen klock-driven:
* `comm::node::Stub::tick()`: Kör nodens logik (parser, hantering, sändning).
* `comm::bus::Stub::tick()`: Levererar bytes som ligger i kö.

Detta gör att vi kan:
* Simulera många noder deterministiskt.
* Skriva testfall som kan repeteras.
* Senare ersätta bussen med verklig hårdvara.

---

### Felmodell och determinism
I en ideal buss gäller att:
* Inga bytes tappas.
* Inga bytes korruptas.
* Ingen fördröjning.

I en mer realistisk simulering kan vi lägga till en felmodell:
* Droppa enstaka bytes.
* Korrupta bytes (bitflip).
* Fördröjning.

För att testfall ska vara reproducerbara behöver felmodellen vara deterministisk.
Exempel:
* Byte nummer 5 droppas alltid i den första sändningen.
* Den första checksummabyten i varannan frame görs alltid korrupt.

---
