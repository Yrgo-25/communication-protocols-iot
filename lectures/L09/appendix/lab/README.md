# Laboration — CAN och CANalyzer

## Syfte
I denna laboration ska ni få:
* Implementera och felsöka CAN-kommunikation.
* Koppla ihop ett fysiskt CAN-nätverk.
* Analysera CAN-trafik med Vector CANalyzer.
* Jämföra CAN med det egna kommunikationsprotokollet från kursen.

Laborationen genomförs i elektronikprojektgrupperna.

---

## Förkunskaper
Innan laborationen ska ni ha:
* Grundläggande förståelse för CAN-protokollet och CAN-frames.
* Installerat Vector CANalyzer.

---

## Material
Varje grupp behöver:
* 2 × Arduino Uno.
* 2 × MCP2515 CAN-moduler/shields (finns på skolan).
* USB-kablar.
* Jumperkablar.
* Två lysdioder med anslutna resistorer (220–330 Ω).
* Två tryckknappar med pull-down-resistorer (10 kΩ). Knapparna ska kopplas aktivt höga.
* Termineringsresistorer (120 Ω).

---

## Förberedelser

### 1. Installera MCP2515-bibliotek
Ladda ner biblioteket [MCP_CAN_LIB](https://github.com/coryjfowler/MCP_CAN_lib#) som en ZIP-fil.

---

### 2. Lägg till biblioteket i Arduino IDE
Öppna Arduino IDE och klicka på följande i det vänstra övre hörnet:

```text
Sketch → Include Library → Add .ZIP Library
```

Välj ZIP-filen som laddades ner från GitHub.

---

### 3. Verifiera installation
Kontrollera att följande inkludering fungerar i en ny sketch:

```cpp
#include <mcp_can.h>
```

---

## Del 1 — Hårdvara (för hårdvarunissarna i Eh25)

### Uppgift 1
Koppla ihop två Arduino-enheter med MCP2515-shields:

Anslut följande:  
**a)** CANH ↔ CANH  
**b)** CANL ↔ CANL  
**c)** GND ↔ GND  

Se undersidan av shielden för information om pins.

### Uppgift 2
Ladda ned CANalyzer på en av era datorer:
* [CANalalyzer Full Installer 18 SP5](https://www.vector.com/int/en/download/canalyzer-full-installer-18-sp5/)
* [Vector Driver Setup 26.10.2 for Windows 10 and 11](https://www.vector.com/int/en/download/vector-driver-setup-26-10-2-for-windows-10-and-11/)

---

## Del 2 — Programvara (för mjukvarunissarna i EE25)

### Uppgift 1
Öppna befintlig [kodbas](./can_iot_lab/can_iot_lab.ino) i Arduino IDE:
* Studera systemlogiken.
* Testkör applikationen på en Arduino (ingen extern hårdvara behövs).
* Notera att stubbar används för tillfället. Knapptryckningar simuleras och data skickas via en simulerad CAN-kontroller.
* Kommentera ut makrot `STUB` i [can_iot_lab.ino](./can_iot_lab/can_iot_lab.ino) för att köra med riktiga drivers. GPIO-drivern finns redan, men CAN-drivern måste ni implementera själva.

### Uppgift 2
Implementera en driver för CAN-kontrollern `MCP2515` döpt `can::driver::Mcp2515`:
* Headerfil: [can_driver_mcp2515.h](./can_iot_lab/can_driver_mcp2515.h).
* Källkodsfil: [can_driver_mcp2515.cpp](./can_iot_lab/can_driver_mcp2515.cpp) innefattande förslag på initiering via metoden `can::driver::Mcp2515::init()`.

Tips på funktioner att använda från det nedladdade biblioteket:

```cpp
MCP_CAN::MCP_CAN(uint8_t chipSelPin);
uint8_t MCP_CAN::checkReceive();
uint8_t MCP_CAN::readMsgBuf(uint32_t *id, uint8_t *len, uint8_t *buf);
uint8_t MCP_CAN::sendMsgBuf(uint32_t id, uint8_t ext, uint8_t len, uint8_t *buf);
```

---

## Del 3 - Testkörning med hårdvara
Efter att ha kopplat upp lysdioderna samt tryckknapparna, ladda upp programvaran till båda Arduino-enheterna.

Verifiera att:
* Knapptryckningar genererar CAN-frames.
* LED-status uppdateras via mottagande av CAN-frames (samt att en tryckknapp styr en LED lokalt).

---

## Del 4 — CANalyzer (under labben)

### Uppgift 1
Anslut Vector CAN-interface till CAN-nätverket:
* Anslut USB till datorn och starta CANalyzer.
* Verifiera att licensen laddas korrekt.

### Uppgift 2
Skapa en ny CANalyzer-konfiguration med:  
**a)** korrekt CAN-kanal.  
**b)** korrekt baudrate.  
**c)** korrekt bussinställning.  

### Uppgift 3
Starta loggning och observera CAN-trafik.

Identifiera följande:  
**a)** CAN-ID.  
**b)** DLC.  
**c)** Payload-data.  
**d)** Sändande nod.  

---

## Examination
Laborationen redovisas på plats. Godkänt (G) ges efter genomförd laboration.

---
