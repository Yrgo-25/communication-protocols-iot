# Kursinfo

## Lärare

Erik Pihl ([erik.axel.pihl@gmail.com](mailto:erik.axel.pihl@gmail.com))

---

# Kursplan – Kommunikationsprotokoll & IoT
Kursen är projektbaserad och fokuserar på kommunikation i inbyggda system. 

Föreläsningarna behandlar kommunikationslager och deras egenskaper, och kopplas direkt till projektet **P02+**.

---

## Föreläsningsplan

| Föreläsning | Datum | Innehåll |
| ----------- | ----- | -------- |
| L01         | 22/4  | Frames (struktur, fält, serialisering) |
| L02         | 29/4  | Byte-parsing och state machine |
| L03         | 5/5   | Databussar och routing (del I), start av **P02+** |
| L04         | 8/5   | Databussar och routing (del II), arbete med **P02+** |
| L05         | 11/5  | Felmodell för databussar (del I), arbete med **P02+** |
| L06         | 11/5  | Felmodell för databussar (del II), arbete med **P02+** |
| L07         | 13/5  | ACK/NACK (del I), arbete med **P02+** |
| L08         | 15/5  | ACK/NACK (del II), arbete med **P02+** |
| L09         | 18/5  | CAN (Controller Area Network) |
| L10         | 19/5  | **Vector CANalyzer-labb (obligatorisk)** |
| L11         | 20/5  | Timeout, retries och dubbletthantering (del I), arbete med **P02+** |
| L12         | 22/5  | Timeout, retries och dubbletthantering  (del II), arbete med **P02+** |
| L13         | 25/5  | Repetition, övningsdugga, arbete med **P02+** |
| L14         | 28/5  | **D01 – Dugga**, arbete med **P02+** |
| L15         | 29/5  | Slutförande och redovisning av **P02+** |

---

---

## Examination och betygsnivåer

### Upplägg
* Ett projekt (**P02+**).
* En skriftlig dugga (**D01**).
* En obligatorisk labb (**CANalyzer**, L10).

---

### Poängfördelning
* **P02+**: upp till 4p (G = 2p, VG = 4p)
* **D01**: upp till 4p (G = 2p, VG = 4p)

Totalt: 8 poäng

**OBS!** CANalyzer-labben är obligatorisk men ger inga poäng.

---

### Betygsnivåer
* 4 ≤ **G** ≤ 6 poäng
* 6 < **VG** ≤ 8 poäng

För godkänt krävs:
* Godkänt resultat på **P02+**.
* Godkänt resultat på **D01**.
* Genomförd labb (**L10**).

---

## Kursmaterial

### Hårdvara
Följande hårdvara finns tillgänglig på skolan:
* **ESP32-S3**
  * Huvudplattform i projektet (**P02+**).
  * Används för implementation av kommunikation (I2C, RS-485, CAN) samt MQTT.
* **RS-485-transceiver ([MAX485](https://www.electrokit.com/rs-485-sandare/mottagare?gad_source=1&gad_campaignid=17338847491&gbraid=0AAAAAD_OrGOlqvD9cVZXO3Az3sdXcfwlw&gclid=EAIaIQobChMIlL3bgI75kgMVmwuiAx0BXSYWEAQYASABEgJxqvD_BwE))**
  * Möjliggör implementation av RS-485-buss.
  * Differential och half-duplex-kommunikation.
* **CAN-transceiver ([SN65HVD230](https://www.amazon.se/-/en/SN65HVD230-Transceiver-Module-Electromagnetic-Reduction/dp/B0GL51N6RC))**
  * Används tillsammans med ESP32-S3:s inbyggda TWAI (CAN-controller).
  * Möjliggör CAN-kommunikation med hårdvaruarbitrering.
* 120 Ω termineringsmotstånd och tvinnad kabel.

---

### Mjukvara
* Visual Studio Code
* ESP-IDF

---