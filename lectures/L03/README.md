# L03 - Databussar och routing (del I)

## Dagordning
* Databuss (översikt).
* Adressering (DST/SRC).
* Simulerad buss i mjukvara:
    * En bus som förmedlar bytes mellan noder.
    * Tick-driven modell (ingen blockering).
* Start av **P02+**:
    * Bygga buss och nod (stubbar).
    * Koppla byte-flöde mellan noder.

---

## Mål med lektionen
* Förstå vad en databuss är och varför broadcast är vanligt i inbyggda system.
* Kunna beskriva hur routing fungerar med DST/SRC.
* Kunna bygga en enkel buss och nodstruktur.
* Kunna skicka och ta emot bytes mellan noder.
* Påbörjad struktur för **P02+**.

---

## Förutsättningar
* L01: `comm::frame::Frame` med `serialize()` och `deserialize()`.
* L02: `comm::frame::Parser` som kan extrahera frames från byte-stream.
* Grundläggande C/C++ (klasser, interface, referenser, arrayer).

---

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_bus_and_routing.md) för information om databussar och routing.
* Läs igenom projektbeskrivningen för **P02+** [här](../../project/README.md).

### Under lektionen
* Genomför övningsuppgifter i [bilaga B](./appendix/b_exercises.md).

---

## Utvärdering
* Förstår ni varför vi behöver DST och SRC?
* Förstår ni varför en buss ofta är broadcast (alla får allt)?
* Kan ni beskriva vad som händer i följande kedja:
    * `Frame::serialize()` → `Bus` → `Node::onRxByte()` → `Parser` → `Frame::deserialize()`.
* Kan ni förklara varför `tick()`-driven design är användbar i simulering?

---

## Nästa lektion
* Fortsättning på dagens tema.
* Fortsatt arbete med **P02+**.

---
