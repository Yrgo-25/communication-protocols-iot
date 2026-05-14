# L06 - Felmodell för databussar (del II)

## Dagordning
* Repetition: Fel i kommunikationen (L05).
* Demonstration:
  * Felmodellering via unit-tester.
  * Simulering av kommunikationsfel med Google Test.
* Analys av fel i byte-ström och frames.
* Fortsatt arbete med **P02+**.

---

## Mål med lektionen
* Förstå hur kontrollerade fel påverkar byte-ström och frames.
* Kunna analysera effekter av:
  * Tappade bytes.
  * Korrupt data.
  * Duplicerade bytes.
* Förstå skillnaden mellan slumpmässiga och kontrollerade (deterministiska) fel.
* Förstå hur unit-tester kan användas för att:
  * Reproducera kommunikationsfel.
  * Verifiera parser-beteende.
  * Testa robusthet i protokollimplementationer.
* Förstå vilka problem som måste lösas för tillförlitlig kommunikation.

---

## Förutsättningar
* Genomförande av L01–L05.

---

## Instruktioner

### Förberedelse
* Installera paket för Google Test i WSL om ni vill vara med och koda live under genomgången:

```bash
sudo apt -y update
sudo apt -y install libgtest-dev libgmock-dev cmake
cd /usr/src/gtest
sudo cmake .
sudo make
sudo mv lib/*.a /usr/lib
cd ~
```

Unit-testerna kommer läggas till i katalogen [testsuite](./testsuite/README.md).

### Under lektionen
* Delta under demonstration av unit-tester och felmodellering för frames samt en frame-parser från övningsuppifterna i [L02](../L02/notes/README.md).
  * Observera hur kontrollerade fel injiceras i byte-strömmen.
  * Analysera parserns beteende vid olika typer av fel.
  * Identifiera eventuella buggar i parsern.
* Fortsätt med **P02+** i grupper.
* Övningsuppgifterna i [bilaga B](../L05/appendix/b_exercises.md) (L05) är valfria och kan användas som stöd eller referens.

---

## Utvärdering
* Beskriv skillnaden i effekt mellan:
  * Tappade bytes.
  * Korrupt data.
  * Fördröjning.
* Beskriv skillnaden mellan slumpmässiga och kontrollerade fel.
* Förklara varför deterministiska tester är viktiga vid utveckling av kommunikationsprotokoll.

---

## Nästa lektion
* Hur gör vi kommunikationen tillförlitlig?
* Introduktion av ACK/NACK.
* Fortsatt arbete med **P02+**.

---
