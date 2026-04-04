# L01 - Frames

## Dagordning
* Kursöversikt.
* Vad är ett kommunikationsprotokoll?
* Problem med ostrukturerad datakommunikation (råa byte-streams utan struktur, synkronisering eller validering).
* Framestruktur: Synkronisering (SOF), längd, payload, checksumma.
* Ett första protokoll: Design och implementation.

---

## Mål med lektionen
* Förstå vad ett kommunikationsprotokoll är och varför det behövs.
* Förstå skillnaden mellan frame och byte-stream.
* Kunna beskriva hur en frame är uppbyggd.
* Kunna implementera en enkel frame i C++.

---

## Förutsättningar
* Grundläggande C/C++ (pekare, arrayer, funktioner och struktar).

---

## Instruktioner

### Förberedelse
* Läs igenom [bilaga A](./appendix/a_frames.md) för information om frames.

### Under lektionen
* Genomför övningsuppgifter i [bilaga B](./appendix/b_exercises.md).

### Demonstration
* En korrekt frame och hur denna packas upp (via deserialisering).
* En frame med trasig checksumma och hur det upptäcks.
* Vanlig bug: endianness (hur `0x0005` kan tolkas som `0x0500`).

---

## Utvärdering
* Förstår ni vad ett kommunikationsprotokoll är?
* Förstår ni varför följande fält behövs:
    * SOF
    * Payload-längden
    * Frame-typen
    * Destinationsadressen
    * Avsändaradressen
    * Sekvensnumret
    * Payloaden
    * Checksumman

---

## Nästa lektion
* Implementation av en byte-stream parser:
    * I denna lektion antar vi att vi alltid får en komplett frame i en buffer. 
    * Nästa lektion behandlar verkligheten, där data överförs byte för byte.

---

