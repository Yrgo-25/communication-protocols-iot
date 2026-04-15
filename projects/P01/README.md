# P01 – IoT Driverintegration

## Översikt
I detta projekt ska ni i grupper om 3-4 designa och implementera ett lagerindelat IoT-system, där en hårdvarunära driver integreras med ett nätverkslager (MQTT). Systemet ska implementeras på ESP32-S3.

Målet är att integrera:
* En driver för ett lägre lager (I2C, CAN eller RS-485).
* Ett nätverkslager (MQTT).

Systemet ska demonstrera hur olika protokollager samverkar i ett strukturerat och lagerindelat inbyggt system.

Projektet bygger vidare på det kommunikationssystem som utvecklas i kursens tidigare moment:
* Frames (L01).
* Parser (L02).
* Buss och noder (L03–L04).

---

## Lärandemål
Efter att ha genomfört **P01** ska ni kunna:
* Implementera en hårdvarunära kommunikationsdriver på ESP32-S3.
* Integrera två protokollager i ett sammanhängande system.
* Designa tydliga och väldefinierade gränssnitt mellan lager.
* Resonera kring adressering, tillförlitlighet och ansvarsfördelning mellan hårdvara och mjukvara.
* Presentera och försvara tekniska arkitekturbeslut muntligt.

---

## Projektstruktur
* Arbeta i grupper om 3-4.
* Båda studenterna ska aktivt delta i implementation och muntlig redovisning.

---

## Systemarkitektur
Ert system ska innehålla följande:

```text
Applikationslogik  
    ↓  
MQTT (nätverkslager)  
    ↓  
Brygglogik  
    ↓  
Driver för ett lägre lager (I2C / CAN / RS-485)  
    ↓  
Fysiskt interface (ESP32-S3 hårdvara)
```

**OBS!**
* Systemet måste visa verklig interaktion mellan lagren.
* Det är inte tillräckligt att implementera två oberoende drivers.

Exempel på uppdelning i kod:
* `driver::` (I2C / CAN / RS-485).
* `comm::` (ramar, parser, ev. buss).
* `app::` (MQTT + logik).

---

## Tillåtna kombinationer
Varje grupp väljer ett lägre lager att kombinera med MQTT. 

Följande kombinationer är tillåtna:
* I2C + MQTT
* CAN + MQTT
* RS-485 + MQTT

Information om respektive protokoll finns bifogat:
* [I2C](./appendix/a_i2c.md)
* [RS-485](./appendix/b_rs485.md)
* [MQTT](./appendix/c_mqtt.md)
* [CAN](./appendix/d_can.md)

---

## Funktionella krav
Systemet ska:
* Använda ESP32-S3-hårdvara.
* Implementera en fungerande driver för ett lägre lager (I2C, CAN eller RS-485).
* Implementera MQTT-kommunikation (WiFi-baserad).
* Koppla samman båda lagren på ett meningsfullt sätt.
* Data ska flöda från det lägre lagret → MQTT eller vice versa.

Exempel:
* Sensorvärde läses via I2C → publiceras via MQTT.
* CAN-meddelande tas emot → vidarebefordras till MQTT-topic.
* MQTT-kommando tas emot → skickas via RS-485 till annan nod.
* Bryggnod som översätter mellan CAN och MQTT.

---

### Krav på driver för det lägre lagret
Er driver ska:
* Använda ESP-IDF eller motsvarande låg-nivå-API.
* Tydligt separera:
  * Hårdvaruabstraktion.
  * Driverlogik.
  * Applikationsanvändning.
* Hantera:
  * Adressering.
  * Sändning.
  * Mottagning.
  * Felhantering (om tillämpligt).

Ni ska kunna förklara:
* Fysisk signalering.
* Arbitreringsmodell (om tillämpligt).
* Vad som hanteras i hårdvara respektive mjukvara.
* Hur fel detekteras och hanteras.

---

### Krav på MQTT-delen
Er MQTT-implementation ska:
* Ansluta till en broker.
* Publicera minst ett topic.
* Prenumerera på minst ett topic.
* Visa dubbelriktad kommunikation.

Ni ska kunna förklara:
* Broker-modellen.
* QoS-nivåer.
* Tillförlitlighetsmekanismer.
* Topic-baserad routing.

---

## Versionshantering och inlämning

Projektet ska versionshanteras i ett Git-repo (t.ex. GitHub eller GitLab).

Krav:

* Arbeta i grupper om 3-4.
* Koden ska ligga i ett privat repo.
* Läraren ska bjudas in som collaborator.
* All utveckling ska ske via Git (inga zip-filer eller liknande).
* Alla studenter ska bidra med egna commits i repot.

README i repot ska innehålla:
* En kort systembeskrivning.
* Kortfattad beskrivning av hur valda protokoll fungerar.
* En enkel arkitekturskiss (ASCII eller bild).
* Kort instruktion för hur systemet körs.


---

## Examination
Projektet examineras genom:
* Kodgranskning via GitHub.
* Live-demonstration.
* Muntlig genomgång och tekniska frågor.

Båda studenterna ska kunna förklara hela systemet.

---

## Bedömning
### G (2 poäng)
För betyget G ska gruppen visa att:
* Systemet fungerar enligt kraven.
* En driver för ett lägre lager (I2C, CAN eller RS-485) är implementerad på ESP32-S3.
* MQTT är korrekt integrerat.
* De två lagren samverkar i systemet.
* Studenterna kan förklara:
  * Systemets övergripande arkitektur.
  * Hur data flödar mellan lagren.
  * Grundläggande egenskaper hos de valda protokollen.

Fokus för G:  
* Funktionalitet.
* Grundläggande förståelse.

---

### VG (4 poäng)
För betyget VG ska gruppen dessutom visa att:
* Arkitekturen är tydligt lagerindelad och genomtänkt.
* Ansvarsfördelning mellan hårdvara och mjukvara kan förklaras.
* Tillförlitlighetsmekanismer i respektive protokoll kan beskrivas.
* Gruppen kan resonera kring:
  * Begränsningar.
  * Möjliga förbättringar.
  * Vad som händer vid fel (t.ex. tappade meddelanden, broker offline, bus-fel).

Fokus för VG:  
* Arkitekturell förståelse
* Teknisk analys.
* Resonemang.

---

### Viktigt
* Det räcker inte att visa att "det fungerar".
* Båda studenterna ska kunna svara på tekniska frågor.
* Kopierad kod utan förståelse ger inte VG.

---

## Slutmål
Efter att ha genomfört **P01** ska ni kunna läsa en protokollspecifikation och förstå var den passar in i en lagerindelad arkitektur.

---
