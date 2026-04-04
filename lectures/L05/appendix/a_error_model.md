# Bilaga A

## Felmodell för databussar
I verkliga embedded-system kan följande inträffa:
* En byte tappas.
* En byte ändras (bitflip).
* Bytes fördröjs.
* Frames kolliderar (vid vissa bussar).

I vår simulering vill vi kunna återskapa detta kontrollerat.

---

### Varför determinism?
Om vi introducerar slumpmässiga fel:
* Tester blir icke-reproducerbara.
* Debugging blir svårt.
* Resultat varierar mellan körningar.

Istället använder vi deterministiska regler:
Exempel:
* Droppa alltid byte nummer 5.
* Korrupta alltid första checksummabyten.
* Fördröj varje byte med 3 tick.

Detta gör att testfall kan upprepas exakt.

---

### Felmodell i bussen
Vi kan utöka `comm::bus::Stub` med:
* En räknare för antal skickade bytes (`myByteCounter`).
* En regelmotor (om X == `myByteCounter` → droppa byten).
* En fördröjningskö i form av en räknare som räknas nedåt (`myDelayCounter`).

Exempel:

```cpp
constexpr std::uint8_t dropByte{5U};

// Drop byte 5.
if (dropByte == myByteCounter) { return true; }
```

eller

```cpp
constexpr std::size_t bit{0U};

// Flip bit 0.
byte ^= (1U << bit);
```

---

### Parserns återhämtning
Parsern ska:
* Förkasta felaktiga frames (vid checksum mismatch).
* Återgå till tillståndet `WaitForSof1`.
* Kunna hitta nästa korrekta frame i byte-strömmen.

Detta är avgörande innan vi bygger vidare med tillförlitlighet.

---
