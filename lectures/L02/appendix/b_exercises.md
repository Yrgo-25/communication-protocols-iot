# Bilaga B

## Övningsuppgifter – Byte-parsing

Följande övningsuppgifter bygger vidare på övningsuppgifterna från 
[L01](../../L01/appendix/b_exercises.md).

### **1.** Implementera en frame-parser
Lägg till filen [comm/frame/parser.h](./code/parser.h) i ert projekt.  
Definiera metoddefinitionerna i en ny fil `comm/frame/parser.cpp`.

Parsern ska:
* Vänta på SOF byte 1.
* Vänta på SOF byte 2.
* Läsa LEN.
* Samla bytes tills hela framen enligt längdfältet har mottagits.
* Markera att en komplett frame har mottagits via `isFrameReady()`.
* Ge tillgång till frame-data via `extractFrame()`, som även validerar framen via 
`Frame::deserialize()`.
* Om framen är ogiltig ska parsern återgå till att söka efter ny SOF.

---

### **2.** Skicka PONG vid mottagande av PING
I en ny fil `comm/frame/handler.h`, deklarera en funktion med namnet `handleFrame()`:

``` cpp
bool handleFrame(const Frame& txFrame, Frame& rxFrame) noexcept;
```

Denna funktion ska läsa `txFrame` och skicka returdata via `rxFrame`:
* Om TYPE == PING:
    * Byt DST/SRC.
    * Behåll SEQ.
    * TYPE = PONG.
    * LEN = 0 (payload tom).
    * Returnera `true`.
* Annars:
    * Returnera `false`.

Definiera `handleFrame()` i en ny fil `comm/frame/handler.cpp`.

---

### **3.** Sammankoppling
Använd bifogat testprogram i [main.cpp](./code/main.cpp):
* 1. Skapa en PING-frame. 
* 2. Serialisera framen. 
* 3. Mata in bytes en och en via en frame parser. 
* 4. När framen har extraherats:
    * Anropa handleFrame().
    * Serialisera PONG.
    * Skriv ut resultat på hexadecimal form.
---

### **4.** Robust SOF-hantering (om tid finns)
Verifiera följande scenario:
* En byte `0xA5` (SOF1) tas emot.
* Nästa byte är inte `0xF7` (SOF2), utan exempelvis `0xA5` igen.
* Parsern ska kunna hantera detta utan att missa en potentiell frame-start:
    * Testa att starta om parsningen med rätt SOF efter att föregående parsning misslyckades.
    * Verifiera att parsningen lyckades.

---
