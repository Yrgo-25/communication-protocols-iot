# Kommunikationsprotokoll & IoT
Repo för kursen **Kommunikationsprotokoll & IoT** med klassen Ee25, vt26.

---

## Om kursen
Kursen är projektbaserad och bygger på att ni stegvis utvecklar ett eget kommunikationssystem i C++.

Föreläsningarna varvar teori och implementation:
* Ni implementerar själva centrala delar av ett protokoll.
* Ni testar och vidareutvecklar systemet under kursens gång.
* Resultatet används i projektet **P01**, där systemet kopplas till verklig hårdvara och MQTT.

Kursen behandlar kommunikation i inbyggda system med fokus på:
* Design av egna kommunikationsprotokoll (framing).
* Byte-stream parsing och robust felhantering.
* Adressering och routing mellan noder.
* Tillförlitlig kommunikation (ACK/NACK, timeout, retry).
* Lagerindelning (Frame → Parser → Applikation).
* Introduktion till fysiska och logiska protokoll (RS-485, CAN, I2C, MQTT).
* Implementation på ESP32-S3.

---

## Vad ni bygger
Under kursens gång utvecklar ni:
* Ett eget frame-baserat protokoll i C++.
* En state-machine parser för byte-strömmar.
* Kommunikation mellan noder:
  * Ping/Pong
  * StatusRequest/StatusResponse
* Tillförlitlighet:
  * Sekvensnummer
  * ACK/NACK
  * Timeout/retry

Denna protokollmodell används som referens när vi studerar verkliga system.

I kursens projekt (**P01**) implementerar ni ett IoT-system där:
* Ett lägre lager (I2C, CAN eller RS-485) används.
* Systemet kopplas till MQTT.
* Data flödar mellan lagren i ett sammanhängande system.

---

## Lärandemål
Efter genomförd kurs ska studenten kunna:
* Förklara vad ett kommunikationsprotokoll är och varför det behövs.
* Förstå och analysera wire-format.
* Implementera serialisering/deserialisering.
* Förstå hur en state-machine parser fungerar.
* Resonera kring adressering, routing och lagerindelning.
* Förstå principerna bakom timeout och retry.
* Förklara skillnaden mellan:
  * Integritet (checksum)
  * Leveransgaranti (ACK/retry)
* Jämföra olika kommunikationsmodeller:
  * RS-485
  * CAN
  * I2C
  * MQTT
* Implementera och integrera kommunikation i ett IoT-system.

---

## Struktur

```text
exam/        Tentameninformation och övningstentamen
info/        Kursinformation, schema och examination
lectures/    Föreläsningar, litteratur och övningsuppgifter
projects/    Projektbeskrivningar, krav och inlämningsinstruktioner
```

---
