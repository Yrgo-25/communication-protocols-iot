# L02 - Byte-parsing

## Dagordning
* Repetition: Frame-formatet och fältens syfte (SOF, LEN, TYPE, DST, SRC, SEQ, DATA, CHK).
* Verkligheten: Data kommer som en byte-stream (inte som en komplett buffer).
* Tillståndsmaskiner (state machines) för parsing.
* Parser-design: processera bytes, extrahera frames med mera.
* Robusthet: Skräpdata, tappade bytes och trasiga frames.
* Demo: PING in byte för byte → PONG tillbaka.

---

## Mål med lektionen
* Förstå varför byte-stream parsing behövs.
* Kunna beskriva en parser som en tillståndsmaskin.
* Kunna implementera en frame-parser som:
    * Tar emot en byte i taget.
    * Kan ta emot en komplett frame (utan skräpdata) och extrahera den korrekt.
* Kunna återhämta sig från skräpdata före en frame.
* Kunna förkasta frames med trasig checksumma.

---

## Förutsättningar
* L01 klar: Ni har en fungerande `comm::frame::Frame` med `serialize()` och `deserialize()`.
* Grundläggande C/C++ (pekare, arrayer, klasser, enum class).

---

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_frame_parsing.md) gällande frame-parsning.

### Under lektionen
* Genomför övningsuppgifter i [bilaga B](./appendix/b_exercises.md).

### Demonstration
* Skräpdata före SOF → parsern hittar SOF och fortsätter.
* Trasig checksumma → parsern avvisar framen.
* Två frames back-to-back → parsern hittar båda.

---

## Utvärdering
* Kan ni förklara varför vi behöver:
    * SOF (synk).
    * LEN (hur många bytes som återstår).
    * En state machine (för att läsa byte för byte).
* Kan ni beskriva vad `processByte()` ska returnera och när?
* Kan ni beskriva när `extractFrame()` får returnera `true`?

---

## Nästa lektion
* Routing via adresser och en simulerad databuss:
    * Nu kan vi redan tolka frames från en byte-stream.
    * Nästa steg är att förmedla bytes mellan flera noder via en buss.

---
