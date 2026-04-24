# P02+ – IoT-extension av driverbiblioteket

## Översikt
I detta projekt ska ni bygga vidare på ert **P02**-projekt från C++-kursen.

Ni ska inte skapa ett nytt system, utan utöka ert befintliga driverbibliotek och systemlogik med kommunikation.

Målet är att koppla ert lokala inbyggda system till ett IoT-sammanhang.

---

## Uppgift

Ni ska lägga till nya drivers i ert **P02**-projekt:
* Ett nätverkslager (MQTT).
* Ett lägre kommunikationslager (I2C, CAN eller RS-485).

**Notering:**
* För betyget **G** räcker det att implementera och integrera MQTT med systemlogiken.
* För betyget **VG** ska även det lägre kommunikationslagret implementeras och integreras.

---

## Krav

### 1. Återanvänd P02
* Er befintliga systemlogik ska användas.
* Er arkitektur med interfaces, factory och drivers ska behållas.
* `main` ska vara "tunn":
  * Endast skapa factory och systemlogik.
  * Starta systemet (t.ex. `logic.run()`).
  * Ingen applikationslogik eller kommunikationslogik får implementeras i `main`.

---

### 2. Nya drivers
Ni ska implementera:
* En MQTT-driver.
* För **VG**: En driver för ett lägre lager (I2C / CAN / RS-485).

Båda ska:
* Vara separerade från systemlogiken.
* Följa samma designprinciper som i **P02**.

---

### 3. Integration
* För **G**: MQTT ska aktivt påverka eller läsa från systemlogiken.
* För **VG**: MQTT och det lägre lagret ska samverka.

---

## Exempel på funktionalitet
Minst ett av följande ska fungera:

### Styrning via MQTT
* `led/on` → LED på (via systemlogik)
* `led/off` → LED av
* `blink/on` → aktivera blink
* `blink/off` → stoppa blink
* `blink/period 1000` → ändra period

---

### Temperatur via MQTT
* Publicera temperatur från TMP36:
  * t.ex. `sensor/temp`

---

### Integration med lägre lager
* MQTT-kommando skickas vidare via CAN / RS-485 / I2C
* Data från lägre lager publiceras via MQTT

---

## Genomförande
Samma regler som i **P02** gäller:
* Samma grupper används.
* Projektet versionshanteras i samma Git-repo.
* Alla studenter ska bidra med egna commits.
* Examination sker via demo, kodgranskning och muntlig genomgång.

---

## Bedömning

### G
* MQTT-driver implementerad.
* MQTT ska påverka eller läsa från P02-systemlogiken (inte bara vara en separat demo).
* Exempel:
  * MQTT-kommando styr LED eller blinkläge.
  * Temperatur från P02 publiceras via MQTT.
* Grundläggande koppling mellan kommunikation och systemlogik.

### VG
* Kraven för G är uppfyllda.
* Även ett lägre kommunikationslager (I2C / CAN / RS-485) implementerat.
* MQTT och det lägre lagret ska samverka i systemet (inte vara separata funktioner).
* Exempel:
  * MQTT → CAN/RS-485/I2C → påverkar systemlogik.
  * Data från lägre lager → publiceras via MQTT.
* Välstrukturerad och lagerindelad arkitektur.

### Bidrag till kursens slutpoäng
* Betyget **G** ger 2 poäng till kurssammanställningen. 
* Betyget **VG** ger 4 poäng till kurssammanställningen.

---
